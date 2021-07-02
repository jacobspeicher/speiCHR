using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows.Media;

namespace speiCHR.Files
{
    class Palette : INotifyPropertyChanged
    {
        private SolidColorBrush[] _colors = new SolidColorBrush[4];
        public event PropertyChangedEventHandler PropertyChanged;

        public Palette()
        {
            Color black = CreateColor(255, 0, 0, 0);
            Color red = CreateColor(255, 255, 0, 0);
            Color green = CreateColor(255, 0, 255, 0);
            Color blue = CreateColor(255, 0, 0, 255);

            Colors[0] = new SolidColorBrush(black);
            Colors[1] = new SolidColorBrush(red);
            Colors[2] = new SolidColorBrush(green);
            Colors[3] = new SolidColorBrush(blue);
        }

        public SolidColorBrush[] Colors
        {
            get { return _colors; }
            set
            {
                _colors = value;
                OnPropertyChanged();
            }
        }

        protected void OnPropertyChanged([CallerMemberName] string name = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        private Color CreateColor(byte A, byte R, byte G, byte B)
        {
            Color color = new Color();

            color.A = A;
            color.R = R;
            color.G = G;
            color.B = B;

            return color;
        }
    }
}
