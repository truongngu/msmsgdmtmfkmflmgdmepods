using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace SpriteEditor
{
    public class Vector2
    {
        float _x, _y;

        public float Y
        {
            get { return _y; }
            set { _y = value; }
        }

        public float X
        {
            get { return _x; }
            set { _x = value; }
        }
        
        public Vector2(float x, float y)
        {
            _x = x;
            _y = y;
        }

        public Vector2(float x)
        {
            _x = x;
            _y = x;
        }

        public Vector2()
        {
            _x = 0;
            _y = 0;
        }

        public override string ToString()
        {
            return _x + " " + _y;
        }
    }

    public class Vector3
    {
        float _x, _y, _z;

        public float Z
        {
            get { return _z; }
            set { _z = value; }
        }

        public float Y
        {
            get { return _y; }
            set { _y = value; }
        }

        public float X
        {
            get { return _x; }
            set { _x = value; }
        }

        public Vector3(float x, float y, float z)
        {
            _x = x;
            _y = y;
            _z = z;
        }

        public Vector3(float x, float y)
        {
            _x = x;
            _y = y;
            _z = 0;
        }

        public Vector3(float x)
        {
            _x = x;
            _y = x;
            _z = x;
        }

        public Vector3()
        {
            _x = 0;
            _y = 0;
            _z = 0;
        }

        public override string ToString()
        {
            return _x + " " + _y + " " + _z;
        }
    }

    public class EntityInfor
    {
        #region Field
        string _texturePath;
        string _className;

        //SPRITE_ATTRIBUTE
        float _fps;
        float _width;
        float _height;
        int _numSpriteX;
        int _numSpriteY;
        int _totalSprite;
        //SPRITE_ATTRIBUTE

        //SHADER
        string _vertexShader;
        string _fragmentShader;
        //SHADER

        //FRAME - ANIMATION
        int _numFrame;
        List<FrameInfor> _frames = new List<FrameInfor>();
        //FRAME - ANIMATION

        //CLONE
        int _numClone;
        List<CloneInfor> _clones = new List<CloneInfor>();
        //CLONE
        #endregion

        #region Properties
        public EntityInfor()
        {

        }

        public EntityInfor(String path2Texture)
        {
            TexturePath = path2Texture;
            ClassName = "AnimatedSprite";
            Fps = 12.0f;
            Width = 1.0f;
            Height = 1.0f;
            NumSpriteX = 1;
            NumSpriteY = 1;
            TotalSprite = 1;
            VertexShader = "Resources/Shaders/SpriteShaderVS.vsh";
            FragmentShader = "Resources/Shaders/SpriteShaderFS.fsh";
            NumFrame = 0;
            NumClone = 1;
            CloneInfor clone = new CloneInfor();
            clone.Name = "";
            clone.X = 0.0f;
            clone.Y = 0.0f;
            clone.Z = 1.0f;
            Clones.Add(clone);
        }

        public string TexturePath
        {
            get { return _texturePath; }
            set { _texturePath = value; }
        }

        public List<CloneInfor> Clones
        {
            get { return _clones; }
            set { _clones = value; }
        }

        public int NumClone
        {
            get { return _numClone; }
            set { _numClone = value; }
        }

        public List<FrameInfor> Frames
        {
            get { return _frames; }
            set { _frames = value; }
        }

        public int NumFrame
        {
            get { return _numFrame; }
            set { _numFrame = value; }
        }

        public string FragmentShader
        {
            get { return _fragmentShader; }
            set { _fragmentShader = value; }
        }

        public string VertexShader
        {
            get { return _vertexShader; }
            set { _vertexShader = value; }
        }

        public string ClassName
        {
            get { return _className; }
            set { _className = value; }
        }

        public float Fps
        {
            get { return _fps; }
            set { _fps = value; }
        }

        public float Width
        {
            get { return _width; }
            set { _width = value; }
        }

        public float Height
        {
            get { return _height; }
            set { _height = value; }
        }

        public int NumSpriteX
        {
            get { return _numSpriteX; }
            set { _numSpriteX = value; }
        }

        public int NumSpriteY
        {
            get { return _numSpriteY; }
            set { _numSpriteY = value; }
        }

        public int TotalSprite
        {
            get { return _totalSprite; }
            set { _totalSprite = value; }
        }
        #endregion

        #region Method
        public void LoadFromXml(XmlNode element)
        {
            string textureXpath = "TEXTURE_PATH";
            TexturePath = element.SelectSingleNode(textureXpath).InnerText;

            string classXpath = "CLASS_NAME";
            ClassName = element.SelectSingleNode(classXpath).InnerText;

            string attrXpath = "SPRITE_ATTRIBUTE";
            XmlNode attrEle = element.SelectSingleNode(attrXpath);

            Fps = float.Parse(attrEle.Attributes["fps"].Value);
            Width = float.Parse(attrEle.Attributes["width"].Value);
            Height = float.Parse(attrEle.Attributes["height"].Value);
            NumSpriteX = int.Parse(attrEle.Attributes["num_spriteX"].Value);
            NumSpriteY = int.Parse(attrEle.Attributes["num_spriteY"].Value);
            TotalSprite = int.Parse(attrEle.Attributes["total_sprite"].Value);

            string shaderXpath = "SHADER";
            attrEle = element.SelectSingleNode(shaderXpath);

            VertexShader = attrEle.Attributes["vertex"].Value;
            FragmentShader = attrEle.Attributes["fragment"].Value;

            string frameXpath = "FRAME_INFO";
            attrEle = element.SelectSingleNode(frameXpath);

            NumFrame = int.Parse(attrEle.Attributes["num_frame"].Value);

            string childFrame = "FRAME";
            XmlNodeList frameNode = attrEle.SelectNodes(childFrame);

            Frames = new List<FrameInfor>();
            int size = frameNode.Count;
            if (size > NumFrame)
                size = NumFrame;
            //foreach (XmlNode node in frameNode)
            for (int i = 0; i < size;i++)
            {
                FrameInfor infor = new FrameInfor();
                infor.LoadFromXml(frameNode[i]);

                Frames.Add(infor);
            }

            string cloneXpath = "CLONE_INFO";
            attrEle = element.SelectSingleNode(cloneXpath);

            NumClone = int.Parse(attrEle.Attributes["num_clone"].Value);

            Clones = new List<CloneInfor>();

            string childClone = "CLONE";
            XmlNodeList cloneNode = attrEle.SelectNodes(childClone);

            size = cloneNode.Count;
            if (size > NumClone)
                size = NumClone;

            for (int i = 0; i < size; i++)
            {
                CloneInfor clone = new CloneInfor();
                clone.LoadFromXml(cloneNode[i]);

                Clones.Add(clone);
            }
        }

        public void WriteToXML()
        {

        }
        #endregion
    }
}
