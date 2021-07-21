using System;
using CommandLineParser;

namespace CommandLineParserTest
{
    enum Animal { Dog, Cat, Bird }
    enum Currency { Dollar, Euro, Yen }

    class CommandLineOptions
    {
        [CommandLineOption]
        string Input1 { get; set; } = string.Empty;
        [CommandLineOption(Order = 1)]
        string Input2 { get; set; } = string.Empty;
        [CommandLineOption("switch1")]  // /switch1 or -switch1
        bool Switch1 { get; set; } = false;
        [CommandLineOption("switch2")]  // /switch2 or -switch2
        bool Switch2 { get; set; } = false;
        [CommandLineOption("string1", HasData = true)]  // /string1:value or /string1=value or -string1:value or -string1=value
        string String1 { get; set; } = string.Empty;
        [CommandLineOption("string2", HasData = true)]  // /string2:value or /string2=value or -string2:value or -string2=value
        string String2 { get; set; } = string.Empty;
        [CommandLineOption("integer1", HasData = true)] // i.e. /integer1:777
        int Integer1 { get; set; } = -1;
        [CommandLineOption("integer2", HasData = true)] // i.e. -integer2=-500
        int Integer2 { get; set; } = 0;
        [CommandLineOption("animal", HasData = true)]   // i.e. /animal:Cat
        Animal Animal { get; set; } = Animal.Dog;
        [CommandLineOption("currency", HasData = true)]   // i.e. -currency=Euro
        Currency Currency { get; set; } = Currency.Dollar;

        public override string ToString()
        {
            return string.Format("{0}={1}", nameof(Input1),   Input1)   + Environment.NewLine +
                   string.Format("{0}={1}", nameof(Input2),   Input2)   + Environment.NewLine +
                   string.Format("{0}={1}", nameof(Switch1),  Switch1)  + Environment.NewLine +
                   string.Format("{0}={1}", nameof(Switch2),  Switch2)  + Environment.NewLine +
                   string.Format("{0}={1}", nameof(String1),  String1)  + Environment.NewLine +
                   string.Format("{0}={1}", nameof(String2),  String2)  + Environment.NewLine +
                   string.Format("{0}={1}", nameof(Integer1), Integer1) + Environment.NewLine +
                   string.Format("{0}={1}", nameof(Integer2), Integer2) + Environment.NewLine +
                   string.Format("{0}={1}", nameof(Animal),   Animal)   + Environment.NewLine +
                   string.Format("{0}={1}", nameof(Currency), Currency) + Environment.NewLine;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            CommandLineOptions cmdOpt = new CommandLineOptions();
            ProgramArgumentsParser.Parse(args, cmdOpt);
            Console.Write(cmdOpt);
        }
    }
}
