using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ContourUI
{
    class UserMessage
    {
        public UserMessage()
        { }
        public UserMessage(MsgBoxType type, string text, MsgBoxButton buttons = MsgBoxButton.Close, double height = 200, double width = 500)
        {
            Type = type;
            Text = text;
            Buttons = buttons;
            BoxHeight = height;
            BoxWidth = width;
        }

        public MsgBoxType Type { get; set; } = MsgBoxType.Warning;
        public string Text { get; set; }
        public MsgBoxButton Buttons { get; set; } = MsgBoxButton.Close;
        public double BoxHeight { get; set; } = 200;
        public double BoxWidth { get; set; } = 500;
    }
}
