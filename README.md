# DirectX11 3D Game Engine by Yang

A 3d renderer engine built from scratch with DirectX11 and Win32

## Description

This renderer engine is a half-year-long personal project following [ChiliTomatoNoodle's Tutorial](https://www.youtube.com/watch?v=_4FArgOX1I4&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&index=2). This document is a detailed review of the significant commits to this repository from the very beginning of the project.

## 1. First commit - a win32 window

![Alt text](./screenshots/2021-8-26.png "The hello world window")

### New files
<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>A</td>
	    <td align="left">
	    	<ul>
	    		<li>aaaaa</li>
	    		<li>bbbbb</li>
	    		<li>ccccc</li>
	    	</ul>
	    </td>
    </tr>   
    <tr>
      <td>B</td>
	    <td align="left">
	    	<ul>
	    		<li>aaaaa</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
    <tr>
      <td>C</td>
	    <td align="left">
	    	<ul>
	    		<li>aaaaa</li>
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