using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace DotNetInprocServer
{
    [ClassInterface(ClassInterfaceType.AutoDispatch)]
    [Guid("DEAC9910-6F19-4135-AB6B-E7A4056ABFD6"), ComVisible(true)]
    [ProgId("SampleDotNetInprocServer.SampleComClass")]
    public class SampleComClass
    {
        [DispId(1)]
        public void SampleComMethod() => MessageBox.Show(nameof(SampleComMethod), nameof(SampleComClass));
    }
}
