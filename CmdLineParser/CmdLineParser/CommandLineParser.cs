using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Reflection;

namespace CommandLineParser
{
    public static class ProgramArgumentsParser
    {
        public static void Parse(IEnumerable<string> cmdLineArgs, object cmdLineOptionsObj)
        {
            int nonSwitchOptionOrder = 0;
            foreach (string cmdLineArg in cmdLineArgs)
            {
                if (string.IsNullOrEmpty(cmdLineArg))
                {
                    continue;
                }
                PropertyInfo pi = null; CommandLineOptionAttribute attr = null; string strValue = null;
                if (cmdLineArg[0] == '-' || cmdLineArg[0] == '/')
                {
                    string[] cmd = cmdLineArg.Substring(1).Split(':', '=');
                    if ((pi = FindPropertyForCommandLineOption(cmd[0], cmdLineOptionsObj, ref attr)) != null)
                    {
                        if (attr.HasData)
                        {
                            if (cmd.Length > 1)
                            {
                                strValue = cmdLineArg.Substring(cmd[0].Length + 2);
                            }
                            else
                            {
                                Report("The option requires data", cmdLineArg);
                            }
                        }
                        else
                        {
                            strValue = "True";   // Simple switch is on
                        }
                    }
                    else
                    {
                        Report("Unrecognized option", cmdLineArg);
                    }
                }
                else // Non-switch argument
                {
                    if ((pi = FindPropertyForCommandLineOption(nonSwitchOptionOrder++, cmdLineOptionsObj, ref attr)) != null)
                    {
                        strValue = cmdLineArg;
                    }
                    else
                    {
                        Report("Garbage in the command line", cmdLineArg);
                    }
                }
                if (pi != null && strValue != null)
                {
                    object propValue = null;
                    try
                    {
                        propValue = TypeDescriptor.GetConverter(pi.PropertyType).ConvertFromString(strValue);
                    }
                    catch (Exception e)
                    {
                        Report("Wrong option data", cmdLineArg, e.Message);
                    }
                    if (propValue != null)
                    {
                        Report("Command line argument", cmdLineArg);
                        pi.SetMethod.Invoke(cmdLineOptionsObj, new object[] { propValue });
                    }
                }
            }
        }

        private static PropertyInfo FindPropertyForCommandLineOption(string cmdLineOptionName, object cmdLineOptionsObj, ref CommandLineOptionAttribute cmdLineOptionAttr)
        {
            foreach (PropertyInfo pi in cmdLineOptionsObj.GetType().GetRuntimeProperties())
            {
                foreach (Attribute attr in pi.GetCustomAttributes(typeof(CommandLineOptionAttribute)))
                {
                    if (string.Equals(cmdLineOptionName, ((CommandLineOptionAttribute)attr).Name, StringComparison.OrdinalIgnoreCase))
                    {
                        cmdLineOptionAttr = (CommandLineOptionAttribute)attr;
                        return pi;
                    }
                }
            }
            return null;
        }

        private static PropertyInfo FindPropertyForCommandLineOption(int order, object cmdLineOptionsObj, ref CommandLineOptionAttribute cmdLineOptionAttr)
        {
            int nonSwitchPropOrder = 0;
            foreach (PropertyInfo pi in cmdLineOptionsObj.GetType().GetRuntimeProperties())
            {
                foreach (Attribute attr in pi.GetCustomAttributes(typeof(CommandLineOptionAttribute)))
                {
                    if (string.IsNullOrEmpty(((CommandLineOptionAttribute)attr).Name))
                    {
                        if (nonSwitchPropOrder++ == order && ((CommandLineOptionAttribute)attr).Order == -1 ||
                            ((CommandLineOptionAttribute)attr).Order == order)
                        {
                            cmdLineOptionAttr = (CommandLineOptionAttribute)attr;
                            return pi;
                        }
                    }
                }
            }
            return null;
        }

        private static void Report(string descript, string data, string additionalInfo = null)
        {
            Trace.WriteLine(string.Format("{0}: {1}: {2}", MethodBase.GetCurrentMethod().DeclaringType.Name, descript, data));
            Trace.WriteLineIf(!string.IsNullOrWhiteSpace(additionalInfo), string.Format("\t{0}", additionalInfo));
        }
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = true)]
    public sealed class CommandLineOptionAttribute : Attribute
    {
        public CommandLineOptionAttribute(string name) { Name = name; }
        public CommandLineOptionAttribute() { } // For non-switch command line parameter
        public string Name { get; private set; } = null;
        public bool HasData { get; set; } = false;
        public int Order { get; set; } = -1;    // For non-switch command line parameter
    }
}
