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
using Xceed.Wpf.Toolkit;

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

        /// <summary>
        /// Color from the color picker
        /// </summary>
        private Color _ColorPickerColor = Color.FromRgb(0, 0 ,0);
        public Color ColorPickerColor
        {
            get { return _ColorPickerColor; }
            set { _ColorPickerColor = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Red color value
        /// </summary>
        private ushort _RedValue = 0;
        public ushort RedValue
        {
            get { return _RedValue; }
            set { _RedValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Green color value
        /// </summary>
        private ushort _GreenValue = 0;
        public ushort GreenValue
        {
            get { return _GreenValue; }
            set { _GreenValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Blue color value
        /// </summary>
        private ushort _BlueValue = 0;
        public ushort BlueValue
        {
            get { return _BlueValue; }
            set { _BlueValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Hue min slider value
        /// </summary>
        private ushort _HueMinValue = 0;
        public ushort HueMinValue
        {
            get { return _HueMinValue; }
            set { _HueMinValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Hue max slider value
        /// </summary>
        private ushort _HueMaxValue = 0;
        public ushort HueMaxValue
        {
            get { return _HueMaxValue; }
            set { _HueMaxValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Saturation min slider value
        /// </summary>
        private ushort _SaturationMinValue = 0;
        public ushort SaturationMinValue
        {
            get { return _SaturationMinValue; }
            set { _SaturationMinValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Saturation max slider value
        /// </summary>
        private ushort _SaturationMaxValue = 0;
        public ushort SaturationMaxValue
        {
            get { return _SaturationMaxValue; }
            set { _SaturationMaxValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Value min slider value
        /// </summary>
        private ushort _ValueMinValue = 0;
        public ushort ValueValue
        {
            get { return _ValueMinValue; }
            set { _ValueMinValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Value max slider value
        /// </summary>
        private ushort _ValueMaxValue = 0;
        public ushort ValueMaxValue
        {
            get { return _ValueMaxValue; }
            set { _ValueMaxValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Period slider value
        /// </summary>
        private ushort _PeriodValue = 0;
        public ushort PeriodValue
        {
            get { return _PeriodValue; }
            set { _PeriodValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Live control checkbox value
        /// </summary>
        private bool _LiveControl = false;
        public bool LiveControl
        {
            get { return _LiveControl; }
            set { _LiveControl = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Music control checkbox value
        /// </summary>
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
