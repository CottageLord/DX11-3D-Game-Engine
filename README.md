# DirectX11 3D Game Engine by Yang

A 3d renderer engine built from scratch with DirectX11 and Win32

## Description

This renderer engine is a half-year-long personal project following [ChiliTomatoNoodle's Tutorial](https://www.youtube.com/watch?v=_4FArgOX1I4&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&index=2). This document is a detailed review of the significant commits to this repository from the very beginning of the project.

## 1. First commit - a win32 window

![Alt text](./Screenshots/2021-8-26.png "The hello world window")

### New files
<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td>SYS_MAIN_Loop.cpp</td>
	    <td align="left">
	    	<ul>
	    		<li>The main loop of the program. Invokes App::Go().</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td>SYS_CLASS_App.h</td>
	    <td align="left">
	    	<ul>
	    		<li>Contains the program behavior in each frame. Responsible for instanciating, updating and rendering objects.</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
      <td>SYS_CLASS_MFException.h</td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from std::exception, the MFException is the base class for all exception handlers in this program (Window, Graphics, Tools, etc.)</li>
	    	</ul>
	    </td>
    </tr>
    <tr>
      <td>SYS_CLASS_Window.h</td>
	    <td align="left">
	    	<ul>
	    		<li>A singleton win32 window class that configures win32 settings and provides message communication between our app and the windows system.z</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
    <tr>
      <td>SYS_CLASS_IO_Mouse/Keyboard.h</td>
	    <td align="left">
	    	<ul>
	    		<li>The IO event handler that are envoked by Window::ProcessMessages. Translate and process the raw data from the OS such that our app can use them intuitively.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### important new system

```cpp
 protected:
     // Parent
     SceneNode* m_parent;
 private:
     // Children holds all a pointer to all of the descendents
     // of a particular SceneNode. A pointer is used because
     // we do not want to hold or make actual copies.
     std::vector<SceneNode*> m_children;
     // The object stored in the scene graph
     Object* m_object;
     // Each SceneNode nodes locals transform.
     Transform m_localTransform;
     // We additionally can store the world transform
     Transform m_worldTransform;
```
### Major updates to existing files