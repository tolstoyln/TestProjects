using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace GraphProcessor
{
    class Program
    {
        static void Main(string[] args)
        {
            Processor p = new Processor();
            p.AddDependency("A", "D");
            p.AddDependency("A", "E");
            p.AddUnit("B");
            p.AddUnit("C");
            p.AddDependency("E", "B");
            p.AddUnit("F");
            p.AddDependency("F", "A");
            p.AddDependency("B", "C");

            p.Start();

            if (System.Diagnostics.Debugger.IsAttached)
            {
                Console.ReadKey();
            }
        }
    }

    class Processor
    {
        public void AddUnit(string name)
        {
            if (!units.ContainsKey(name))
            {
                units[name] = new ProcUnit(name);
            }
        }
        public void AddDependency(string prntName, string chldName)
        {
            AddUnit(prntName);
            AddUnit(chldName);
            units[chldName].AddParent(units[prntName]);
        }
        public void Start()
        {
            /*List<Task> tasks = new List<Task>();
            foreach (KeyValuePair<string, ProcUnit> pu in units)
            {
                tasks.Add(Task.Run(() => pu.Value.Start()));
            }
            Task.WaitAll(tasks.ToArray());*/
            Parallel.ForEach(units, pu => pu.Value.Start());
            Console.WriteLine("All completed");
        }
        private Dictionary<string, ProcUnit> units = new Dictionary<string, ProcUnit>();
    }

    class ProcUnit
    {
        public ProcUnit(string name) { Name = name; }
        public string Name { get; private set; }
        public class Result { }
        public event Action<string, Result> ProcessComplete;
        public void AddParent(ProcUnit pu)
        {
            if (!parents.ContainsKey(pu.Name))
            {
                pu.ProcessComplete += (name, res) =>
                {
                    parents[name] = res;
                    Interlocked.Decrement(ref parentCount);
                };
                ++parentCount;
                parents[pu.Name] = null;
            }
        }
        public void Start()
        {
            while (Interlocked.CompareExchange(ref parentCount, 0, 0) > 0);
            Process();
        }
        /*public void Start()
        {
            Task.Run(() =>
            {
                while (Interlocked.CompareExchange(ref parentCount, 0, 0) > 0);
                Process();
            });
        }*/
        /*public async void Start()
        {
            await Task.Run(() => { while (Interlocked.CompareExchange(ref parentCount, 0, 0) > 0); });
            //_ = Task.Factory.StartNew(() => { Process(); });
            _ = Task.Run(() => { Process(); });
        }*/
        /*private void OnParentProcessComplete(string name, Result res)
        {
            parents[name] = res;
            Interlocked.Decrement(ref parentCount);
        }*/
        private void Process()
        {
            Console.WriteLine(string.Format("{0} processing started - ThreadId: {1}", Name, Thread.CurrentThread.ManagedThreadId));
            Thread.Sleep(1000);
            Console.WriteLine(string.Format("{0} processing completed", Name));
            ProcessComplete?.Invoke(Name, new Result());
        }
        private int parentCount = 0;
        private ConcurrentDictionary<string, Result> parents = new ConcurrentDictionary<string, Result>();
    }
}
