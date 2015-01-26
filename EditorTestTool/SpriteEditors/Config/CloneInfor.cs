using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace SpriteEditor
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class CloneInfor
    {
        #region Field
        string _name;
        float _x;
        float _y;
        float _z;
        float _userData;
        #endregion

        #region Properties
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }
        
        public float X
        {
            get { return _x; }
            set { _x = value; }
        }
       
        public float Y
        {
            get { return _y; }
            set { _y = value; }
        }
       
        public float Z
        {
            get { return _z; }
            set { _z = value; }
        }
       
        public float UserData
        {
            get { return _userData; }
            set { _userData = value; }
        }
        #endregion

        #region Method
        public void LoadFromXml(XmlNode element)
        {
            Name = element.Attributes["name"].Value;
            X = float.Parse(element.Attributes["x"].Value.Replace(".",","));
            Y = float.Parse(element.Attributes["y"].Value.Replace(".", ","));
            Z = float.Parse(element.Attributes["z"].Value.Replace(".", ","));
            UserData = float.Parse(element.Attributes["usedata"].Value.Replace(".", ","));

            MainWindow.cloneMap[Name] = this;
        }
        #endregion

        public void WriteToXml(XmlElement clone)
        {
            clone.SetAttribute("name", Name);
            
            clone.SetAttribute("x", X.ToString().Replace(",","."));
            clone.SetAttribute("y", Y.ToString().Replace(",","."));
            clone.SetAttribute("z", Z.ToString().Replace(",", "."));
            clone.SetAttribute("usedata", UserData.ToString().Replace(",", "."));
        }
    }
}
