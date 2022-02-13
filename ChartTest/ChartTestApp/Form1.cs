using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using PulseDataProvider;

namespace ChartTestApp
{
    public partial class Form1 : Form
    {
        private DataProvider _dataProvider = new DataProvider();
        private List<int> _data = new List<int>();
        private bool Started { get; set; } = false;

        public Form1()
        {
            InitializeComponent();

            Size szScreen = Screen.PrimaryScreen.WorkingArea.Size;
            Size = new Size(szScreen.Width * 2 / 3, szScreen.Height * 2 / 3);
        }

        private void OnLoad(object sender, EventArgs e)
        {
            Application.Idle += Application_Idle;

            cmbInterval.SelectedIndex = 0;
            cmbMode.SelectedIndex = 0;

            _dataProvider.OnPulseData += (val) =>
            {
                _data.Add(val);
                FillChart();
            };
            _dataProvider.OnTestFinished += () => Started = false;

            FillChart();
        }

        private void Application_Idle(object sender, EventArgs e)
        {
            btnStartStop.Text = Started ? Properties.Resources.Button_Stop : Properties.Resources.Button_Start;
            cmbInterval.Enabled = !Started;

            int last = _data.LastOrDefault();
            bool bNormal = _data.Count == 0 || (last >= 60 && last <= 120);
            lblBpm.BackColor = bNormal ? Color.Lime : Color.FromArgb(255, 255, 102, 102);
            lblBpm.Text = string.Format($"{last} bpm");
        }

        private void StartStorp_OnClick(object sender, EventArgs e)
        {
            if (!Started)
            {
                _data.Clear();
                _dataProvider.Start(this, 3600, Interval);
            }
            else
            {
                _dataProvider.Stop();
            }
            Started = !Started;
        }

        private void OnViewModeChange(object sender, EventArgs e)
        {
            FillChart();
        }

        private void FillChart()
        {
            chart.ChartAreas[0].AxisX.Minimum = OriginTime;
            chart.ChartAreas[0].AxisX.Maximum = OriginTime + ViewMode;
            int dt = ViewMode / TimeSpans;
            chart.ChartAreas[0].AxisX.Interval = dt;
            chart.ChartAreas[0].AxisX.CustomLabels.Clear();
            for (int t = (int)chart.ChartAreas[0].AxisX.Minimum; t <= chart.ChartAreas[0].AxisX.Maximum; t += dt * 2)
            {
                chart.ChartAreas[0].AxisX.CustomLabels.Add(t - dt * 2, t + dt * 2, string.Format($"{(t / 60):D2}:{(t % 60):D2}"));
            }
            chart.Series[0].Points.Clear();
            chart.Series[0].Points.AddXY(chart.ChartAreas[0].AxisX.Minimum - Interval, 0);
            for (int i = (int)chart.ChartAreas[0].AxisX.Minimum / Interval; i < _data.Count; ++i)
            {
                chart.Series[0].Points.AddXY(i * Interval, _data[i]);
            }
        }

        private readonly int[] _intervals = { 10, 5, 2, 1 };
        private readonly int[] _modes = { 120, 300, 1200, 3600 };
        private int Interval { get => _intervals[cmbInterval.SelectedIndex]; }
        private int ViewMode { get => _modes[cmbMode.SelectedIndex]; }
        private int TimeSpans { get => cmbMode.SelectedIndex == 0 || cmbMode.SelectedIndex == 3 ? 12 : 10; }
        private int OriginTime { get => Math.Max(0, (_data.Count - 1) * Interval - ViewMode); }
    }
}
