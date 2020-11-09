Praxis3D converter
===================
&nbsp;  

Praxis3D-converter is a tool for converting various assets to be used in Praxis3D game engine. It currently supports conversions of maps (along with all the assets) from [ProjectExodus](https://github.com/NegInfinity/ProjectExodus) exported scenes, allowing Praxis3D to import maps from Unity and Unreal Engine.  
&nbsp;  
&nbsp;  
![praxis3d-window](https://i.imgur.com/0V7MRrk.png)

## Building

Projects is self-sufficient - all dependencies are included and all settings within the Visual Studio project files are configured for an out of the box compilation in **Visual Studio 2019**.

## Usage

- Select an import file (.JSON file exported from [ProjectExodus](https://github.com/NegInfinity/ProjectExodus));   
- Select an export directory (where the Praxis3D scene will be saved);  
- Click **Convert**.  
  
You can also choose an additional directory that contains all the assets (if the exported map from [ProjectExodus](https://github.com/NegInfinity/ProjectExodus) did not copy the original assets).


![praxis3d-window2](https://i.imgur.com/AuRT1Gf.png)

## Dependencies
Libraries used in this project:
- [Open-Asset-Importer-Lib (ASSIMP)](https://github.com/assimp/assimp)  
- [FreeImage](https://freeimage.sourceforge.io/)  
- [Nlohmann JSON](https://github.com/nlohmann/json)  
    
Their individual licenses can be found in **license.txt** file.


## License

**The software is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License:**

Copyright © 2020 Paulius Akulavicius

Permission is hereby granted, free of charge, 
to any person obtaining a copy of this software 
and associated documentation files (the “Software”), 
to deal in the Software without restriction, 
including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions 
of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.