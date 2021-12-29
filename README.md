# DirectX11 3D Game Engine by Yang

A 3d renderer engine built from scratch with DirectX11 and Win32

## Description

This renderer engine is a half-year-long personal project following [ChiliTomatoNoodle's Tutorial](https://www.youtube.com/watch?v=_4FArgOX1I4&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&index=2). This document is a detailed review of the significant commits to this repository from the very beginning of the project.

## Commit 1 - a win32 window with customized IO and Exception system

![Alt text](./Notes/1.jpg "Window and message system")

![Alt text](./Notes/2.jpg "Component object model")

![Alt text](./Notes/3.jpg "DirectX 11 Interface and graphics pipeline")

![Alt text](./Screenshots/2021-8-26.png "The hello world window")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_MAIN_Loop.h">SYS_MAIN_Loop.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_MAIN_Loop.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The main loop of the program. Invokes App::Go().</li>
	    		<li>Do the outmost exception handling.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_Graphics.h">SYS_CLASS_Graphics.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_Graphics.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A D3D11 Graphic Devices manager, configures swap chain, basic screnn buffer, sampler(anti-aliasing) and so on. </li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_App.h">SYS_CLASS_App.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_App.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Contains the program behavior in each frame. Responsible for instanciating, updating and rendering objects.</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_MFException.h">SYS_CLASS_MFException.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_MFException.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from std::exception, the MFException is the base class for all exception handlers in this engine (Window, Graphics, Tools, etc.)</li>
	    	</ul>
	    </td>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_Window.h">SYS_CLASS_Window.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_Window.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A singleton win32 window class that configures win32 settings.</li>
	    		<li>Provides message communication between our app and the windows system.</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Mouse.h">SYS_CLASS_IO_Mouse.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Mouse.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Keyboard.h">Keyboard.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/a0dc6d71be91d79cad91605654188ed17470a14b/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Keyboard.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The IO event handlers that are envoked by Window::ProcessMessages. </li>
	    		<li>Translate and process the raw data from the OS such that our app can use them more intuitively.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates to existing files

None.

## Commit 2 - The first triangle with shaders

![Alt text](./Screenshots/2021-8-28.png "The first shape")

### New files

None.

### Major updates to existing files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/f1642d0470232aeb334f447a8bd99ef40d53cfdc/DX11%203D%20Game%20Engine/SYS_CLASS_Graphics.h">SYS_CLASS_Graphics.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/f1642d0470232aeb334f447a8bd99ef40d53cfdc/DX11%203D%20Game%20Engine/SYS_CLASS_Graphics.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Added test triangle drawer. Tested the procedure of configuring and feeding the rendering pipeline. </li>
	    		<li>This process will be constantly modulared, generalized and optimized in the future commits. </li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 3 - The bindable system - Reusability, Flexibility and Persistence

![Alt text](./Notes/4.jpg "System layout")

![Alt text](./Screenshots/2021-8-30.png "Duplicate geometries with shared data")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Drawable.h">GRAPHICS_OBJ_Drawable.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Drawable.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Represents a drawble entity such as a box.</li>
	    		<li>Manages a collection of "bindables" objects that could be binded abd shipped to rendering pipeline, such as vertex buffer, index buffer, transforms, shader constants and other topology settings.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Bindable.h">GRAPHICS_OBJ_Bindable.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Bindable.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The base class (interface) for all bindable objects.</li>
	    		<li>The pure virtual function Bind(Graphics& gfx) expects different implementations from its children (how vertex buffer and shader objects bind themselves respectively).</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_BUF_VertexBuffer.h">GRAPHICS_BUF_VertexBuffer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_BUF_VertexBuffer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Bindable.</li>
	    		<li>Takes in a vector of vertecies, store it as a member, and create buffer for D3D11_SUBRESOURCE_DATA as D3D11_BIND_VERTEX_BUFFER in the gfx device.</li>
	    		<li>Bind the vertex using IASetVertexBuffers()</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_BUF_IndexBuffer.h">GRAPHICS_BUF_IndexBuffer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_BUF_IndexBuffer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Bindable.</li>
	    		<li>Take in a vector of indecies, store it as a member, and create buffer for D3D11_SUBRESOURCE_DATA as D3D11_BIND_INDEX_BUFFER in the gfx device.</li>
	    		<li>Bind the index using IASetIndexBuffer()</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_PixelShader.h">GRAPHICS_OBJ_PixelShader.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_PixelShader.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_VertexShader.h">VertexShader.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_VertexShader.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Bindable.</li>
	    		<li>Take in the compiled-shader-file paths and register the corresponding shader objects.</li>
	    		<li>Bind the shaders using VS | PSSetShader().</li>
	    		<li>The vertex shader stores the input vertecies bytecode for InputLayout registration.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_InputLayout.h">GRAPHICS_OBJ_InputLayout.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_InputLayour.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Bindable.</li>
	    		<li>Take in the bytecode that will be sent to the Input Assembly stage, register the input layout by CreateInputLayout().</li>
	    		<li>Bind the input layout using IASetInputLayout()</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_BUF_ConstantBuffers.h">GRAPHICS_BUF_ConstantBuffers.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Bindable. Contains ConstantBuffer, VertexConstBuffer and PixelConstBuffer classes.</li>
	    		<li>Take in the raw constants data, like struct with rgba floats, and register them to respective shader constants.</li>
	    		<li>Bind the input layout using VS | PSSetConstantBuffers()</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_BUF_TransformCBuffer.h">GRAPHICS_BUF_TransformCBuffer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_BUF_TransformCBuffer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Bindable.</li>
	    		<li>Take in the current view-space-transform of the draw target, apply the projection to it.</li>
	    		<li>Bind the final transform matrix as VertexConstant to the shader.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Topology.h">GRAPHICS_OBJ_Topology.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Topology.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Bindable.</li>
	    		<li>Bind the Topology setting using IASetPrimitiveTopology, almost always D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_StaticDrawInfo.h">GRAPHICS_OBJ_StaticDrawInfo.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Drawable. Sit between the Drawable base and the actual drawable geometry object.</li>
	    		<li>Ultilizaing the feature of the template programming. By using AddStaticBind() instead of the AddBind(), making sure that all Drawable(Box)s share the data including Vertex/Index data, Shaders, Constants, etc.</li>
	    		<li>The hierarchy graph in the note above shows how the system eventually looklike. The Box code below also provides the example in practice.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Box.h">GRAPHICS_OBJ_Box.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Box.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Inherit from Drawable.</li>
	    		<li>Instanciate a painted box and bind all data/settings using the bindable system.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/SYS_SET_GraphicsThrowMacros.h">SYS_SET_GraphicsThrowMacros.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/SYS_SET_WindowsThrowMacros.h">SYS_SET_WindowsThrowMacros.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Holds respective exception detection MACROS.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates to existing files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/60004ef793a5d01cc0c3e2567d7f2e17575bc6ad">Commit detail</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Reorganized the exception detection MACROS.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 4 - Play with various geometries

![Alt text](./Screenshots/2021-9-2.png "Different geometries")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/9b67f91b49b388257430ce1c24f85058d083a592/DX11%203D%20Game%20Engine/SYS_SET_Math.h">SYS_SET_Math.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Some math helpers that performs angle warp around, angle to radient and basic linear interpolation.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/9b67f91b49b388257430ce1c24f85058d083a592/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_IndexedTriangleList.h">GRAPHICS_OBJ_IndexedTriangleList.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>An encapsulation of paired index and vertex buffers.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/9b67f91b49b388257430ce1c24f85058d083a592/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Cube.h">GRAPHICS_OBJ_Cube.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/9b67f91b49b388257430ce1c24f85058d083a592/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Cone.h">GRAPHICS_OBJ_Cone.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/9b67f91b49b388257430ce1c24f85058d083a592/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Prism.h">GRAPHICS_OBJ_Prism.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/9b67f91b49b388257430ce1c24f85058d083a592/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Plane.h">GRAPHICS_OBJ_Plane.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/9b67f91b49b388257430ce1c24f85058d083a592/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Sphere.h">GRAPHICS_OBJ_Sphere.h</a>
      	</td>
	    <td align="left">
	    	<ul>
	    		<li>Play with various drawables.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates to existing files

None.

## Commit 5 - The first triangle with shaders

![Alt text](./Notes/5.jpg "Texture mapping and normal mapping")

![Alt text](./Screenshots/2021-9-4.png "Texture mapping")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Surface.h">GRAPHICS_OBJ_Surface.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Surface.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A surface loader/modifier using Gdiplus. Convert image file into color buffer.</li>
	    		<li>Contains Exception handler and a Color class as a handler into the bit data.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_SET_GDIPlusManager.h">GRAPHICS_SET_GDIPlusManager.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_SET_GDIPlusManager.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Configures the Gdiplus.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Texture.h">GRAPHICS_OBJ_Texture.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Texture.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Take in the loaded Surface, register the D3D11_TEXTURE2D_DESC as D3D11_SHADER_RESOURCE_VIEW_DESC.</li>
	    		<li>Configures texture settings like mipmap levels.</li>
	    		<li>Bind to shader constant by PSSetShaderResources().</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Sampler.h">GRAPHICS_OBJ_Sampler.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/591c72dabee734b9c9317fa12990800ad4baf264/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Sampler.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Configures the sampler settings like filter mode and wrapping mode.</li>
	    		<li>Bind to shader constant by PSSetSamplers().</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates to existing files

None.
