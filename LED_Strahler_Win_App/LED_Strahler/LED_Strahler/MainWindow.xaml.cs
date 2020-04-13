namespace LED_Strahler_GUI
{
    using System;
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Data;
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
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

        private readonly LEDStrahlerBackend Backend = null;

        #region Properties

        private List<LEDStrahler> _DeviceList = new List<LEDStrahler>();
        public List<LEDStrahler> DeviceList
        {
            get { return _DeviceList; }
            set { _DeviceList = value; NotifyPropertyChanged(); }
        }

        private List<string> _ComPortList = new List<string>();
        public List<string> ComPortList
        {
            get { return _ComPortList; }
            set { _ComPortList = value; NotifyPropertyChanged(); }
        }

        private readonly List<LEDGroupControl> _GroupControls = new List<LEDGroupControl>();
        public List<LEDGroupControl> GroupControls
        {
            get { return _GroupControls; }
        }

        #endregion

        #region Constructor

        public MainWindow()
        {
            InitializeComponent();

            //Create Backend
            this.Backend = new LEDStrahlerBackend(this);

            //Link Group controls
            this._GroupControls.Add(this.GroupControl0);
            this._GroupControls.Add(this.GroupControl1);
            this._GroupControls.Add(this.GroupControl2);
            this._GroupControls.Add(this.GroupControl3);
            this._GroupControls.Add(this.GroupControl4);
            this._GroupControls.Add(this.GroupControl5);
        }

        #endregion

    }


    public class AvailableGroups
    {
        public List<string> Groups { get; set; } = new List<string>()
        {
            "Broadcast",
            "Group 1",
            "Group 2",
            "Group 3",
            "Group 4",
            "Group 5"
        };
    }


    public class DoubleToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ((double)value).ToString("F1") + "°C";
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return 0.0;
        }
    }


    public class UintToHexStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ((uint)value).ToString("X8");
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return 0;
        }
    }


    public class IntToGroupConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            var Group = new AvailableGroups();
            return Group.Groups[(int)(byte)value];
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            var Group = new AvailableGroups();
            return Group.Groups.IndexOf(value.ToString());
        }
    }
}
