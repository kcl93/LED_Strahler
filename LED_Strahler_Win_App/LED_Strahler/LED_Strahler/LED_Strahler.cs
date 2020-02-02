﻿namespace LED_Strahler_GUI
{
    using System;
    using System.Windows;
    using System.Collections.Generic;
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

        public LED_Strahler_Serial Serial { get; set; } = new LED_Strahler_Serial();

        #region Properties

        private uint _UUID = 0;
        public uint UUID
        {
            get { return _UUID; }
            set { _UUID = value; NotifyPropertyChanged(); }
        }

        private int _Group = 0;
        public int Group
        {
            get { return _Group; }
            set
            {
                if(value != _Group)
                {
                    _Group = value;
                    Serial.SetGroup((byte)value, this.UUID);
                    NotifyPropertyChanged();
                }
            }
        }

        private double _Temperature = 0.0;
        public double Temperature
        {
            get { return _Temperature; }
            set { _Temperature = value; NotifyPropertyChanged(); }
        }

        private bool _TestState = false;
        public bool TestState
        {
            get { return _TestState; }
            set { _TestState = value; TestStateChanged(value); NotifyPropertyChanged(); }
        }

        public List<string> AvailableGroups { get; set; } = new List<string>()
        {
            "Broadcast",
            "Group 1",
            "Group 2",
            "Group 3",
            "Group 4",
            "Group 5"
        };
        #endregion

        #region Constructor
        public LED_Strahler(uint ID)
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
