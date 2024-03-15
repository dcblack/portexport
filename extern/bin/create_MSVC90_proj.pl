#! /usr/bin/perl -w
#
## Create a MicroSoft Visual Studio solution file

my @include_dirs = (
# Enter include directory paths (absolute or relative) as a list of quoted words, e.g.
#qw'C:\apps\systemc_tlm\tlm',
#qw'..\user'
);

#read additional include and library directories from command line (prefix libs with -L)
@include_dirs = (@include_dirs, @ARGV);

my $index = 0;
foreach (@include_dirs) {
  if ( /-L(.*)/ ) {
    push @library_dirs, $1;
    delete $include_dirs[$index];
  }
  $index++;
}

my $include_dirs = join(';',@include_dirs);
my $library_dirs = join(';',@library_dirs);

print 
  "Adding the following include directories to path:\n$include_dirs","\n"
  if $include_dirs;

print 
  "Adding the following library directories to path:\n$library_dirs","\n"
  if $library_dirs;


my $localdir = ( split('\\\\',qx/cd/) )[-1];  #extract local dir from cd command
chomp $localdir;
#print $localdir;

#create project file
my $project_file = $localdir . ".vcproj";
open (FILE,">$project_file");
select FILE;

while ( <*.cpp> ) {
  #print "found $_ \n";
  push @source_files,$_;
  }

while ( <*.h> ) {
  #print "found $_ \n";
  push @header_files,$_;
  }

while ( <*.hpp> ) {
  #print "found $_ \n";
  push @header_files,$_;
  }


my $sflag;
my $hflag;
foreach my $line(<DATA>) {

  $line=~s/project_name/$localdir/;
  
  if ($line=~/(.*)(AdditionalIncludeDirectories=)(.*)/) {
    my $line_start = $1 . $2;
    my $addI = $3;
    $addI=~/(.*)(\")/ or $include_dirs or next;  #jump to next line if no include files
    if ($2 and $include_dirs) {
      $line = $line_start . $1 . ";" . $include_dirs . "\"\n";
    }
    else {
      $include_dirs and $line = $line_start . "\"" . $include_dirs . "\"\n";
      }
  }
  
  if ($line=~/(.*)(AdditionalLibraryDirectories=)(.*)/) {
    my $line_start = $1 . $2;
    my $addI = $3;
    $addI=~/(.*)(\")/ or $library_dirs or next;  #jump to next line if no library files
    if ($2 and $include_dirs) {
      $line = $line_start . $1 . ";" . $library_dirs . "\"\n";
    }
    else {
      $library_dirs and $line = $line_start . "\"" . $library_dirs . "\"\n";
      }
  }
  
  
  #detect end of Source File Filter block
  if ($sflag && $line=~/(.*)<\/Filter>/) {
  $sflag = 0;
    my $leadtabs = $1;
    foreach $sfile(@source_files) {
      print "$leadtabs\t<File\n";
      print "$leadtabs\t\tRelativePath=\".\\$sfile\">\n";
      print "$leadtabs\t</File>\n";
    }
  }

  #detect end of Header File Filter block
  if ($hflag && $line=~/(.*)<\/Filter>/) {
  $hflag = 0;
    my $leadtabs = $1;
    foreach $hfile(@header_files) {
      print "$leadtabs\t<File\n";
      print "$leadtabs\t\tRelativePath=\".\\$hfile\">\n";
      print "$leadtabs\t</File>\n";
    }
  }
     
  print $line;
  $sflag = 1 if ($line=~/Source Files/);  #start of Source File Filter block
  $hflag = 1 if ($line=~/Header Files/);  #start of Header File Filter block


}

exit 0

__END__
<?xml version="1.0" encoding="Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"
	Name="project_name"
	Keyword="Win32Proj">
	<Platforms>
		<Platform
			Name="Win32"/>
	</Platforms>
	<Configurations>
		<Configuration
			Name="Debug|Win32"
			OutputDirectory="Debug"
			IntermediateDirectory="Debug"
			ConfigurationType="1"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="/vmg"
				Optimization="0"
				AdditionalIncludeDirectories="$(SYSTEMC_HOME)\..\src"
				PreprocessorDefinitions="WIN32;_DEBUG;_CONSOLE;_CRT_SECURE_NO_DEPRECATE;SC_INCLUDE_DYNAMIC_PROCESSES"
				MinimalRebuild="true"
				BasicRuntimeChecks="3"
				RuntimeLibrary="1"
				RuntimeTypeInfo="true"
				UsePrecompiledHeader="0"
				WarningLevel="3"
				Detect64BitPortabilityProblems="false"
				DebugInformationFormat="4"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="systemc.lib"
				OutputFile="$(OutDir)/test1.exe"
				LinkIncremental="2"
				AdditionalLibraryDirectories="$(SYSTEMC_HOME)\SystemC\Debug"
				GenerateDebugInformation="true"
				ProgramDatabaseFile="$(OutDir)/test1.pdb"
				SubSystem="1"
				TargetMachine="1"
			/>
			<Tool
				Name="VCMIDLTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebDeploymentTool"
			/>
			<Tool
				Name="VCManagedWrapperGeneratorTool"
			/>
			<Tool
				Name="VCAuxiliaryManagedWrapperGeneratorTool"
			/>
		</Configuration>
		<Configuration
			Name="Release|Win32"
			OutputDirectory="Release"
			IntermediateDirectory="Release"
			ConfigurationType="1"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="/vmg"
				AdditionalIncludeDirectories="$(SYSTEMC_HOME)\..\src"
				PreprocessorDefinitions="WIN32;NDEBUG;_CONSOLE;_CRT_SECURE_NO_DEPRECATE;SC_INCLUDE_DYNAMIC_PROCESSES"
				MinimalRebuild="true"
				BasicRuntimeChecks="0"
				RuntimeLibrary="0"
				BufferSecurityCheck="false"
				RuntimeTypeInfo="true"
				UsePrecompiledHeader="0"
				WarningLevel="3"
				Detect64BitPortabilityProblems="false"
				DebugInformationFormat="3"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="systemc.lib"
				OutputFile="$(OutDir)/test1.exe"
				LinkIncremental="1"
				AdditionalLibraryDirectories="$(SYSTEMC_HOME)\SystemC\Release"
				GenerateDebugInformation="false"
				SubSystem="1"
				OptimizeReferences="2"
				EnableCOMDATFolding="2"
				TargetMachine="1"
			/>
			<Tool
				Name="VCMIDLTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebDeploymentTool"
			/>
			<Tool
				Name="VCManagedWrapperGeneratorTool"
			/>
			<Tool
				Name="VCAuxiliaryManagedWrapperGeneratorTool"
			/>
		</Configuration>
	</Configurations>
	<References>
	</References>
	<Files>
		<Filter
			Name="Source Files"
			Filter="cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
			UniqueIdentifier="{4FC737F1-C7A5-4376-A066-2A32D752A2FF}">
		</Filter>
		<Filter
			Name="Header Files"
			Filter="h;hpp;hxx;hm;inl;inc;xsd"
			UniqueIdentifier="{93995380-89BD-4b04-88EB-625FBE52EBFB}">
		</Filter>
		<Filter
			Name="Resource Files"
			Filter="rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx"
			UniqueIdentifier="{67DA6AB6-F800-4c08-8B7A-83BB121AAD01}">
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
