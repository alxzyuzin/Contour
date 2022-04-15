using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ContourUI
{
    class UserMessage
    {
        public UserMessage(MsgBoxType type, string text, MsgBoxButton buttons = MsgBoxButton.Close, double height = 200, double width = 500)
        {
            Type = type;
            Text = text;
            Buttons = buttons;
            BoxHeight = height;
            BoxWidth = width;
        }

        public MsgBoxType Type { get; private set; }
        public string Text { get; private set; }
        public MsgBoxButton Buttons { get; private set; }
        public double BoxHeight { get; private set; }
        public double BoxWidth { get; private set; }
    }
}
