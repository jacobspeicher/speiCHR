using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text;

namespace speiCHR.Files
{
    class CHRFile : INotifyPropertyChanged
    {
        private string _name;
        private int[,] _pixels;
        public event PropertyChangedEventHandler PropertyChanged;
        
        public CHRFile()
        {
            Name = "<no file>";
        }

        public CHRFile(string n)
        {
            Name = n;
        }

        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
                OnPropertyChanged();
            }
        }

        public int[,] Pixels
        {
            get { return _pixels; }
            set
            {
                _pixels = value;
                OnPropertyChanged();
            }
        }

        protected void OnPropertyChanged([CallerMemberName] string name = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }
}
