using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace LED_Strahler_GUI
{
    /// <summary>
    /// Interaktionslogik für LEDGroupControl.xaml
    /// </summary>
    public partial class LEDGroupControl : UserControl, INotifyPropertyChanged
    {
        #region Property changed

        public event PropertyChangedEventHandler PropertyChanged;

        // This method is called by the Set accessor of each property.  
        // The CallerMemberName attribute that is applied to the optional propertyName  
        // parameter causes the property name of the caller to be substituted as an argument.  
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

        #region Properties

        /// <summary>
        /// Group name of the control
        /// </summary>
        private string _Group = "Broadcast";
        public string Group
        {
            get { return _Group; }
            set { _Group = value; NotifyPropertyChanged(); }
        }

        private ushort _RedValue = 0;
        public ushort RedValue
        {
            get { return _RedValue; }
            set { _RedValue = value; NotifyPropertyChanged(); }
        }

        private ushort _GreenValue = 0;
        public ushort GreenValue
        {
            get { return _GreenValue; }
            set { _GreenValue = value; NotifyPropertyChanged(); }
        }

        private ushort _BlueValue = 0;
        public ushort BlueValue
        {
            get { return _BlueValue; }
            set { _BlueValue = value; NotifyPropertyChanged(); }
        }

        private ushort _HueMinValue = 0;
        public ushort HueMinValue
        {
            get { return _HueMinValue; }
            set { _HueMinValue = value; NotifyPropertyChanged(); }
        }

        private ushort _HueMaxValue = 0;
        public ushort HueMaxValue
        {
            get { return _HueMaxValue; }
            set { _HueMaxValue = value; NotifyPropertyChanged(); }
        }

        private ushort _SaturationMinValue = 0;
        public ushort SaturationMinValue
        {
            get { return _SaturationMinValue; }
            set { _SaturationMinValue = value; NotifyPropertyChanged(); }
        }

        private ushort _SaturationMaxValue = 0;
        public ushort SaturationMaxValue
        {
            get { return _SaturationMaxValue; }
            set { _SaturationMaxValue = value; NotifyPropertyChanged(); }
        }

        private ushort _ValueMinValue = 0;
        public ushort ValueValue
        {
            get { return _ValueMinValue; }
            set { _ValueMinValue = value; NotifyPropertyChanged(); }
        }

        private ushort _ValueMaxValue = 0;
        public ushort ValueMaxValue
        {
            get { return _ValueMaxValue; }
            set { _ValueMaxValue = value; NotifyPropertyChanged(); }
        }

        private ushort _PeriodValue = 0;
        public ushort PeriodValue
        {
            get { return _PeriodValue; }
            set { _PeriodValue = value; NotifyPropertyChanged(); }
        }

        private bool _LiveControl = false;
        public bool LiveControl
        {
            get { return _LiveControl; }
            set { _LiveControl = value; NotifyPropertyChanged(); }
        }

        private bool _MusicControl = false;
        public bool MusicControl
        {
            get { return _MusicControl; }
            set { _MusicControl = value; NotifyPropertyChanged(); }
        }

        #endregion

        public LEDGroupControl()
        {
            InitializeComponent();
        }
    }
}
