using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace SpriteEditor
{
    public class FrameInfor
    {
        #region Field
        string _name;
        List<int> _frameContent = new List<int>();
        #endregion

        #region Properties
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public List<int> FrameContent
        {
            get { return _frameContent; }
            set { _frameContent = value; }
        }
        #endregion

        #region Methods
        public void LoadFromXml(XmlNode element)
        {
            //get attribute name
            Name = element.Attributes["name"].Value;
            
            //get frame content
            string content = element.InnerText;

            //split into List
            string[] arr = content.Split(' ');

            for (int i = 0; i < arr.Length; i++) {
                FrameContent.Add(int.Parse(arr[i]));
            }
        }
        #endregion
    }
}
