using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpriteEditor
{
    [RefreshProperties(System.ComponentModel.RefreshProperties.All)]
    class HandleInfor : INotifyPropertyChanged
    {
        public int x, y, w, h;

        public int Y
        {
            get { return y; }
            set { y = value; }
        }

        public int W
        {
            get { return w; }
            set { w = value; }
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        [RefreshProperties(System.ComponentModel.RefreshProperties.Repaint)]
        public int H
        {
            get { return h; }
            set { h = value; }
        }

        public int X
        {
            get { return x; }
            set { x = value; }
        }
        public IntPtr hWnd, hinstance;

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
