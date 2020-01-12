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

        #region Properties
        public List<LED_Strahler> DeviceList { get; set; } = new List<LED_Strahler> { };
        public List<string> AvailableGroups { get; set; } = new List<string>()
        {
            "No group",
            "Group 1",
            "Group 2",
            "Group 3",
            "Group 4",
            "Group 5"
        };
        #endregion

        #region Constructor
        public MainWindow()
        {
            InitializeComponent();

            DeviceList.Add(new LED_Strahler(123456789));
            DeviceList.Add(new LED_Strahler(987654321));
            DeviceList.Add(new LED_Strahler(123581347));


        }
        #endregion
    }
}
