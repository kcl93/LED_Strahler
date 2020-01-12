namespace LED_Strahler_GUI
{
    using System;
    using System.Windows;
    using System.Collections;
    using System.Windows.Controls;
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    public class LED_Strahler : INotifyPropertyChanged
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

        private int _UUID = 0;
        public int UUID
        {
            get { return _UUID; }
            set { _UUID = value; NotifyPropertyChanged(); }
        }

        private int _Group = 0;
        public int Group
        {
            get { return _Group; }
            set { _Group = value; NotifyPropertyChanged(); }
        }

        private bool _TestState = false;
        public bool TestState
        {
            get { return _TestState; }
            set { _TestState = value; TestStateChanged(value); NotifyPropertyChanged(); }
        }
        #endregion

        #region Constructor
        public LED_Strahler(int ID)
        {
            this.UUID = ID;
        }
        #endregion

        #region Methods

        private void TestStateChanged(bool state)
        {

        }

        #endregion
    }
}
