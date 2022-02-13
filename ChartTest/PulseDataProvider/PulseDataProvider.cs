using System;
using System.ComponentModel;

namespace PulseDataProvider
{
    public class DataProvider
    {
        public event Action<int> OnPulseData;
        public event Action OnTestFinished;

        public void Start(ISynchronizeInvoke sync, int duration, int interval)
        {
            _duration = duration * 1000;
            _count = 1;
            OnPulseData?.Invoke(_rand.Get());
            _timer = new System.Timers.Timer(interval * 1000);
            _timer.SynchronizingObject = sync;
            _timer.Elapsed += OnTimer;
            _timer.Start();
        }

        public void Stop()
        {
            _timer.Stop();
        }

        private void OnTimer(object source, System.Timers.ElapsedEventArgs e)
        {
            OnPulseData?.Invoke(_rand.Get());

            if (++_count >= _duration / _timer.Interval)
            {
                Stop();
                OnTestFinished?.Invoke();
            }
        }

        private int _count = 0;
        private int _duration = 0;
        private System.Timers.Timer _timer = null;
        class RandomGenerator
        {
            public RandomGenerator(int min, int max) { _min = min; _max = max + 1; }
            public int Get() => _rand.Next(_min, _max);
            private int _min, _max;
            private Random _rand = new Random(DateTime.Now.Millisecond);
        }
        private RandomGenerator _rand = new RandomGenerator(0, 200);
    }
}
