//  +-------------------------------------------------------------------------------------+
//  |SC_MODULE(Top) {                                                                     |
//  |  SC_CTOR(Top) {                                                                     |
//  |    initiator.p1.bind( target.x1 );                                                  |
//  |  }                                                                                  |
//  |                                                                                     |
//  |  Initiator SC_NAMED(initiator);           Target SC_NAMED(target);                  |
//  |  +-------------------------------------+  +---------------------------------------+ |
//  |  |SC_MODULE(Initiator) {               |  |SC_MODULE(Target) {                    | |
//  |  | SC_CTOR(Initiator) {                |  |  SC_CTOR(Target) {                    | |
//  |  |   sc_port<IF> SC_NAMED(p1);         |  |    sc_export<IF> SC_NAMED(x1);        | |
//  |  |   caller.p0.bind( p1 );             |  |    x1.bind( callee.x0 );              | |
//  |  | }                                   |  |  }                                    | |
//  |  |                                     |  |     // Hierarchical channel           | |
//  |  | Caller SC_NAMED(caller);            |  |     Callee SC_NAMED(callee);          | |
//  |  | +-----------------------------+     |  |     +-------------------------------+ | |
//  |  | |SC_MODULE(Caller) {          |     |  |     |SC_MODULE(Callee) : IF {       | | |
//  |  | |  SC_CTOR(Caller) {          |     |  |     |  SC_CTOR(Callee) {            | | |
//  |  | |    sc_port<IF> SC_NAMED(p0);|     |  |     |    sc_export<IF> SC_NAMED(x0);| | |
//  |  | |  //SC_HAS_PROCESS(Caller);  |     |  |     |    x0.bind(*this);            | | |
//  |  | |    SC_THREAD(thread);       |     |  |     |  }                            | | |
//  |  | |  }                          |     |  |     |    // Implementation          | | |
//  |  | | .---------------------.     |     |  |     |    .----------------------.   | | |
//  |  | | | void thread()       |     |     |  |     |    | void xfer(Data& d)   |   | | |
//  |  | | | {                   |     |     |  |     |    | {                    |   | | |
//  |  | | |   p0->xfer(v);      |  [p0]->[p1]->[x1]->[x0] |   m_rcvd = data;     |   | | |
//  |  | | |                     |     |     |  |     |    |   data = m_sent;     |   | | |
//  |  | | | }                   |     |     |  |     |    | }                    |   | | |
//  |  | | '---------------------'     |     |  |     |    '----------------------'   | | |
//  |  | |                             |     |  |     |                               | | |
//  |  | |                             |     |  |     | Data m_rcvd;                  | | |
//  |  | |                             |     |  |     | Data m_sent;                  | | |
//  |  | |                             |     |  |     |                               | | |
//  |  | |};                           |     |  |     |};                             | | |
//  |  | +-----------------------------+     |  |     +-------------------------------+ | |
//  |  |                                     |  |                                       | |
//  |  |};                                   |  |};                                     | |
//  |  +-------------------------------------+  +---------------------------------------+ |
//  |                                                                                     |
//  |};                                                                                   |
//  +-------------------------------------------------------------------------------------+

#include <systemc>
#include <vector>
#include <string>
#include <string_view>
#include "sc_format.hpp"

using namespace sc_core;
using Data = std::string;

struct IF : virtual sc_interface
{
  virtual void xfer( Data& data ) = 0;
};

SC_MODULE( Caller ) {
  static constexpr const char* msg_type = "/Doulos/Example/Ports-n-Exports/Caller";
  sc_port<IF> SC_NAMED(p0);
  explicit SC_CTOR( Caller ) {
    //SC_HAS_PROCESS( Caller );
    SC_THREAD( thread1 );
  }
  void thread1()
  {
    SC_REPORT_INFO( fmt::format("{}/{}",msg_type,__func__).c_str(), fmt::format("Initiating process").c_str() );
    std::vector<Data> datavec = { "Hello", "World" };
    for( auto& v : datavec ) {
      SC_REPORT_INFO( fmt::format("{}/{}",msg_type,__func__).c_str(), fmt::format("sending {}",v).c_str() );
      p0->xfer( v );
      SC_REPORT_INFO( fmt::format("{}/{}",msg_type,__func__).c_str(), fmt::format("received {}",v).c_str() );
    }
    sc_core::sc_stop();
  }
};

// Hierarchical channel
struct Callee : sc_module, private IF {
  static constexpr const char* msg_type = "/Doulos/Example/Ports-n-Exports/Callee";
  sc_export<IF> SC_NAMED(x0);
  explicit Callee( sc_module_name const& instance ) // Constructor
  : sc_module{instance}, IF{}
  {
    x0.bind(*this);
  }
  void xfer( Data& data ) override
  {
    SC_REPORT_INFO( fmt::format("{}/{}",msg_type,__func__).c_str(), fmt::format("received {}",data).c_str() );
    // Save/load data
    auto temp = data;
    data = m_data;
    // Transform data
    if( temp == "Hello" ) temp = "Goodbye";
    m_data = temp;
  }
private:
  Data m_data{ "What's up?" };
};

SC_MODULE( Initiator ) {
  sc_port<IF> SC_NAMED(p1);
  Caller      SC_NAMED(caller);
  explicit SC_CTOR( Initiator ) {
    caller.p0.bind( p1 );
  }
};

SC_MODULE( Target ) {
  sc_export<IF> SC_NAMED(x1);
  Callee        SC_NAMED(callee);
  explicit SC_CTOR( Target ) {
    x1.bind( callee.x0 );
  }
};

SC_MODULE( Top ) {
  Initiator SC_NAMED(initiator);
  Target    SC_NAMED(target);
  explicit SC_CTOR( Top ) {
    initiator.p1.bind( target.x1 );
  }
};

[[maybe_unused]]
int sc_main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[])
{
  Top SC_NAMED(top);
  sc_core::sc_start();
  return 0;
}

// The end
