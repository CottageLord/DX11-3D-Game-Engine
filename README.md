# DirectX11 3D Game Engine by Yang

A 3d renderer engine built from scratch with DirectX11 and Win32

## Description

This renderer engine is a half-year-long personal project following [ChiliTomatoNoodle's Tutorial](https://www.youtube.com/watch?v=_4FArgOX1I4&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&index=2). This document is a detailed review of the significant commits to this repository from the very beginning of the project.

Latest commits are shown first.

![Alt text](./Notes/logo-gray.png "LOGO")

## Commit 26 SkyBox

![Alt text](./Screenshots/2022-5-14.png "Sky Box")

## Commit 23, 24, 25 - Shadow Mapping, Percentage Close Filtering and Sloped-Scaled Shadow Bias

Implemented shadow map and Percentage Close Filtering (PCF) to the sponza scene. Shadow map technique was configured during model loading and controllable during the rendering. Users can specify which objects cast shadows and which receive. Applied shadow bias and PCF for better shadow performances.

Current render passes (implemented in <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/GRAPHICS_RG_BlurOutlineRenderGraph.cpp">GRAPHICS_RG_BlurOutlineRenderGraph.cpp</a>):

1. Generate shadow map
2. Lambertian
3. Outline mask (generate stencil buffer)
4. Gauss blur (for outline effect)
5. Camera wireframes and frustums

More detailed note about shadow mapping: <a href="https://github.com/CottageLord/CS_Courses/blob/main/Notes/GAMES101/Class%20Note%206_%20Shadow%20Map.pdf"> GAMES 101 lecture note: shadow mapping. </a>

![Alt text](./Screenshots/2022-4-16.gif "Shadow map and PCF implemented.")

![Alt text](./Notes/18.jpg "Shadow Mapping")

![Alt text](./Notes/19.jpg "Self shadowing, Peter Panning and PCF")

![Alt text](./Notes/20.jpg "Sloped-Scaled Shadow Mapping")

### New files

Basic shadow mapping

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_ShadowCameraCBuffer.h">GRAPHICS_OBJ_ShadowCameraCBuffer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_ShadowCameraCBuffer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Contains transform/projection information for a shadow camera. This is the essential information for depth comparision during mapping.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_ShadowSampler.h">GRAPHICS_OBJ_ShadowSampler.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_ShadowSampler.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Configures a sampler for sampling shadow map.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/ShadowTest_VS.hlsl">ShadowTest_VS.hlsl</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/ShadowTest_PS.hlsl">ShadowTest_PS.hlsl</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A testing shader that allows objects from receiving shadows.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/PlaceHolder.h">PlaceHolder.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/d1be40dd51935cd19ae9bec7230eee037e7ba678/DX11%203D%20Game%20Engine/PlaceHolder.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li></li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

Percentage Close Filtering

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/bdba84156b1b00f93dfd9dde44bdc31fc5eb309e/DX11%203D%20Game%20Engine/_PShadow_Static.hlsli">_PShadow_Static.hlsli</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A compute shader that samples shadow map and produces soft shadow. Static means the sample range is fixed.</li>
	    		<li>Uses [unroll] to ultilize hardware acceleration.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/bdba84156b1b00f93dfd9dde44bdc31fc5eb309e/DX11%203D%20Game%20Engine/_PShadow_Dynamic.hlsli">_PShadow_Dynamic.hlsli</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A compute shader that samples shadow map and produces soft shadow. Dynamic means the sample range is passed through parameter.</li>
	    		<li>Combines [unroll] and if statements to ultilize hardware acceleration while allowing dynamic inputs.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/bdba84156b1b00f93dfd9dde44bdc31fc5eb309e/DX11%203D%20Game%20Engine/PShadow.hlsli">PShadow.hlsli</a></td>
	    <td align="left">
	    	<ul>
	    		<li></li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

Sloped-Scaled Shadow Bias

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/bdba84156b1b00f93dfd9dde44bdc31fc5eb309e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_ShadowRasterizer.h">GRAPHICS_OBJ_ShadowRasterizer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/bdba84156b1b00f93dfd9dde44bdc31fc5eb309e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_ShadowRasterizer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Encapsulates DirectX settings for sloped scaled shadow bias.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/d1be40dd51935cd19ae9bec7230eee037e7ba678">Basic Shadow Mapping</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Basic shadow map implemented on cubes. Problems including self shadowing still exists.</li>
	    	</ul>
	    </td>
	</tr>	
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/bdba84156b1b00f93dfd9dde44bdc31fc5eb309e">Advanced Shadow Mapping</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Advanced shadow mapping techniques including PCF and Slope Scaled Shadow Bias.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 22 - Depth sampling from camera view

Added depth sampling from any camera. This is the prerequisite for shadow maps.

![Alt text](./Screenshots/2022-4-13.png "A sampled depth from the light-camera.")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/3d99733136a23f8161cfaad40f06357c962bdb5a/DX11%203D%20Game%20Engine/GRAPHICS_PASS_ShadowMappingPass.h">GRAPHICS_PASS_ShadowMappingPass.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Configures a depth map generator from the light-camera perspective.</li>
	    		<li>Registers global render graph source for future shadow map sampling.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/3d99733136a23f8161cfaad40f06357c962bdb5a/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Channels.h">GRAPHICS_OBJ_Channels.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/3d99733136a23f8161cfaad40f06357c962bdb5a/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Channels.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Only contains 2 binary masks - normal rendering or shadow depth sampling.</li>
	    		<li>When submitting different tasks (either Main or Shadow), corresponding techniques will be used for rendering. Techniques contains pipeline information like depth stencil settings and shader constants settings.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/3d99733136a23f8161cfaad40f06357c962bdb5a/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DepthStencil.h">GRAPHICS_OBJ_DepthStencil.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/3d99733136a23f8161cfaad40f06357c962bdb5a/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DepthStencil.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Now allows different usages of depth stencil. Configures corresponding data types for corresponding data storage.</li>
	    		<li>Allows fetching depth map data from gpu side and output image files for dubugging.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/3d99733136a23f8161cfaad40f06357c962bdb5a">Depth sampling</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Cameras can sample depth map and produce texture for future references.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 21 - Multuple Cameras

Added supports for setting multiple cameras. This is a prerequisite for shadow mapping.

![Alt text](./Screenshots/2022-4-11.gif "Controllable multiple cameras")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_PASS_WireframePass.h">GRAPHICS_PASS_WireframePass.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A render pass that draws camera indicators (a camera shape) and frustums (a camera view zone) for all inactive cameras</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_CameraContainer.h">GRAPHICS_OBJ_CameraContainer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_CameraContainer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A manager that holds references to all available camaras.</li>
	    		<li>Capable of spawning control widgets for each camera, allowing users to switch between different camaras and modify their properties.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Projection.h">GRAPHICS_OBJ_Projection.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Projection.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A seperated class for holding each camera's projection information.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Frustum.h">GRAPHICS_OBJ_Frustum.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Frustum.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A drawable object. Represents a camera's view zone.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_CameraIndicator.h">GRAPHICS_OBJ_CameraIndicator.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/8a4636e3af319d0215144b6bcc1bc279b333ad9f/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_CameraIndicator.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A drawable object. Represents a camera's current position.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/173662e402d785e7a3f87d615e9c3c9154764033">Reorganized shader files, fixed bindable pool bug when resolving inputlayout</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The former Bindable Pool register an input layout by its data type definitions. However, DirectX defines input layouts along with vertex shaders. During that process, if the input vertex data structure has more data types than shader needs, the returned input layout will discard those non-neccesary types. In this case, for example, if we define an input layout with both position and color, but construct DirectX layout with a position only shader, the retured layout only contains position layout. If we then use this layout in a position-color shader, error will happen. </li>
	    		<li>Shaders now have better include structures, which reduced code redundancy.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/8a4636e3af319d0215144b6bcc1bc279b333ad9f">Multiple cameras</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Allowing multiple controllable cameras. Nota that only one camera can be set as the main render camera. </li>
	    	</ul>
	    </td>
	</tr>
	
  </tbody>
</table>

## Commit 20 - Render Graph

A render graph (or frame graph) is a way of defining the rendering pipeline using self-contained nodes in an acyclic directed graph. Each node has a set of input and outputs, which link to other nodes or resources. When executed, the graph is traversed executing each node in turn. In the current version, every node represents a pass.

The render graph system takes in nodes (passes) with input/output and validates/links the nodes together. This system allows us to avoid hardcoding the render procedure, but configure it io a graph-like, more organized and automatic way.

To avoid name conflicts, the input and the output for each pass is named as sink (required input type for this pass) and source (output type from this pass).

![Alt text](./Notes/16.jpg "Basic Render Graph")

After this update, the current render process (in <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/SYS_CLASS_App.cpp">App.cpp</a>) is:

1) Engine Initialization(): Create a render graph, which initializes of all passes, creates and links all sinks/sources. Note that the sequenec of passes are specified by the order in a vector.

2) Engine Initialization(): Import models. The <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Material.cpp">Material.cpp</a> automatically assign pass type (by universal pass name strings like "lambertian") to the leaf meshes according to the resource setting. 

3) Engine Start(): The model calls LinkTechniques(RenderGraph), passing the render graph all the way to the mesh leaves. When LinkTechniques() eventually hit the leaves, a leaf will notify its techniques and steps to get the real Pass reference from the render graph (previously just a name string assigned by Material.cpp).

4) Engine Update(): All models will call Submit() to notify all leaf -> Technique -> Step to submit a job (contains the drawable with recent updates like transforms) to the target pass (reference got from previous step).

5) Engine Update(): All the renderGraph.Execute() to render all passes and their jobs.

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_Sink.h">GRAPHICS_RG_Sink.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_Sink.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_Source.h">GRAPHICS_RG_Source.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_Source.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Represents an input (sink) or an output (source) of a pass.</li>
	    		<li>Memorizing the links by storing the self name, pass name and the target (sink or source) name.</li>
	    		<li>Has 3 variants:</li>
	    		<li>1) DirectBindableSink/Source: requires a specific sink/resource in the render graph, hence the data cannot be shared and delivered to mulltiple render graph nodes.</li>
	    		<li>2) DirectBufferSink/Source:  requires a general sink.resource in the render graph, hence the data can be shared and and delivered to mulltiple render graph nodes.</li>
	    		<li>3) ContainerBindableSink: Only exists in BindingPass. Maintains a bindable[] and an index of the empty alot in the bindable[] that expect input from the incoming source. We do this in such way to prevent invalid pointer when we only stores the shared_ptr to the empty slot of a bindable[], and the bindable[] get resized.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_RenderGraph.h">GRAPHICS_RG_RenderGraph.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_RenderGraph.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Base class of all other render graphs.</li>
	    		<li>Manages the global sinks and sources, like the back buffer.</li>
	    		<li>Link the sinks and sources for the current passes. Syntax: SetSinkTarget("sinkName", "anotherPassName.sourceName"). Passes come with their own sinks and sources (currently defined in the constructor).</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_BlurOutlineRenderGraph.h">GRAPHICS_RG_BlurOutlineRenderGraph.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_BlurOutlineRenderGraph.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_ScaleOutlineRenderGraph.h">GRAPHICS_RG_ScaleOutlineRenderGraph.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_RG_ScaleOutlineRenderGraph.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Different render graphs with respective global sink/source settings and linkings.</li>
	    		<li>Passes and their relationships are built in the constructors.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_RenderQueuePass.h">GRAPHICS_PASS_RenderQueuePass.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_RenderQueuePass.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_BindingPass.h">GRAPHICS_PASS_BindingPass.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_BindingPass.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_BufferClearPass.h">GRAPHICS_PASS_BufferClearPass.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_BufferClearPass.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_FullscreenPass.h">GRAPHICS_PASS_FullscreenPass.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_FullscreenPass.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_HorizontalBlurPass.h">GRAPHICS_PASS_HorizontalBlurPass.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_HorizontalBlurPass.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_VerticalBlurPass.h">GRAPHICS_PASS_VerticalBlurPass.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/57e78e18c414ac732d943d4cb0564074a02df4e4/DX11%203D%20Game%20Engine/GRAPHICS_PASS_VerticalBlurPass.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Sets up each pass' unique bindables, constants (i.e Gauss) and render targets (i.e the fullscreen texture). </li>
	    		<li>Configures the Execute() function for each pass. </li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/57e78e18c414ac732d943d4cb0564074a02df4e4">Render Graph</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Many updates to previous codes for incorperating the render graph in the current system.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 19 - Fullscreen Filtering and Sepatared Gauss Blur Effect

Applying blur effect to the pipeline output image. Due to the complex nature of applying outline effect in geometry space (model pivot not centered, mesh not contineous, cannot combine alpha channel, etc.), we now apply the outline effect to the screen space directly.

![Alt text](./Notes/14.jpg "FullScreen Filtering")

![Alt text](./Notes/15.jpg "Separated Gauss Blur Effect")

![Alt text](./Screenshots/2022-1-16.png "Blur effect on non-contineous meshes.")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/ac93d5eea3f15d56856efce3e5ff40722a320f02/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DepthStencil.h">GRAPHICS_OBJ_DepthStencil.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/ac93d5eea3f15d56856efce3e5ff40722a320f02/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DepthStencil.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Separated manager class for configuring depth stencil.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/ac93d5eea3f15d56856efce3e5ff40722a320f02/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_GraphicsResource.h">GRAPHICS_OBJ_GraphicsResource.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/ac93d5eea3f15d56856efce3e5ff40722a320f02/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_GraphicsResource.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Separated manager for accessing DirectX graphic device and context.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/ac93d5eea3f15d56856efce3e5ff40722a320f02/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_RenderTarget.h">GRAPHICS_OBJ_RenderTarget.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/ac93d5eea3f15d56856efce3e5ff40722a320f02/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_RenderTarget.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Separated render target as we now need more render target other than the ones in the swap chain.</li>
	    		<li>It is basically a texture like object to draw dinal images on.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/332de1db63eeb99ca8189e8af77e08d24d2fd55e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_BlurPack.h">GRAPHICS_OBJ_BlurPack.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/332de1db63eeb99ca8189e8af77e08d24d2fd55e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_BlurPack.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Configures the math for Separated Gauss Blur Effect.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/ac93d5eea3f15d56856efce3e5ff40722a320f02">FullScreen Filters</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Take the output from the pipeline as the input got fullscreen filter.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/332de1db63eeb99ca8189e8af77e08d24d2fd55e/DX11%203D%20Game%20Engine/GRAPHICS_JOB_FrameCommander.h">GRAPHICS_JOB_FrameCommander.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/332de1db63eeb99ca8189e8af77e08d24d2fd55e/DX11%203D%20Game%20Engine/GRAPHICS_JOB_FrameCommander.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Integrated the new blur functionality using the extra render target.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/332de1db63eeb99ca8189e8af77e08d24d2fd55e">Separated Gauss Blur Effect</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Added several shader files to incorporate Gauss blur into the rendering.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 18 - Probe System: control every mesh node

This update extends the job system and allows automatic job detection by "probes" traverling in the model-hierarchy. The probe system has two major updates: 1) when hitting the leaf mesh, auto-gen job. 2) when hitting the leaf mesh, auto-gen controls in the UI menu.

![Alt text](./Notes/13.jpg "Probe System")

![Alt text](./Screenshots/2022-1-1.png "UI options for every sub-meshes through probe system")

![Alt text](./Screenshots/2021-12-31.gif "Controlleable sub-meshes and techniques through probe system")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_JOB_ModelProbe.h">GRAPHICS_JOB_ModelProbe.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_JOB_ModelProbe.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The base class for all probes. The probes travel through the model node hierarchy and take respective actions when hitting a leaf.</li>
	    		<li>Probes are passed recursively like ModelRoot.Accept( probe ).</li>
	    		<li>Model Probe is responsible for generating UI handles to the render options that Technique Probe generated.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/3cc28dc14d2f63b809eafd7f583fc0b83841a5ab/DX11%203D%20Game%20Engine/GRAPHICS_JOB_TechniqueProbe.h">GRAPHICS_JOB_TechniqueProbe.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/3cc28dc14d2f63b809eafd7f583fc0b83841a5ab/DX11%203D%20Game%20Engine/GRAPHICS_JOB_TechniqueProbe.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Base class for technique probes. Provides logical handles to leaf mesh render styles.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/3cc28dc14d2f63b809eafd7f583fc0b83841a5ab/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Material.h">GRAPHICS_OBJ_Material.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/3cc28dc14d2f63b809eafd7f583fc0b83841a5ab/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Material.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Initializes all techniques needed for loading materials (labertian, stencil, outline...).</li>
	    		<li>Automatically assign techniques to model leaves according to supplied material data types. For example, assign a alpha testing pixel shader bindable object when alpha channel detected in textures.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/da79d07bfceea556167b2730d6b894718158e5ec">Basic probe system</a></td>
	    <td align="left">
	    	<ul>
	    		<li>This update provides accesses to model leaves' render styles (bindables, vertex data, etc.). This is essential for the later UI update.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Model.h">GRAPHICS_OBJ_Model.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Model.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Node.h">GRAPHICS_OBJ_Node.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Node.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Integrated into the Job System.</li>
	    		<li>The node can now accept a probe, pass it to children nodes and allows probes to get information/update data.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DynamicVertex.h">GRAPHICS_OBJ_DynamicVertex.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/da79d07bfceea556167b2730d6b894718158e5ec/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DynamicVertex.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Uses the MACROS trick as in the Commit 16. Reduced code amount.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/3cc28dc14d2f63b809eafd7f583fc0b83841a5ab">UI controll for model nodes.</a></td>
	    <td align="left">
	    	<ul>
	    		<li>This update enables users to tweak every individual item's render style in the scene.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>



## Commit 17 - Job System and Multi-pass Rendeing

In previous code, meshes like the sponza scene are configured and rendered as a whole, which does not support scene graph which allows modification on sub-meshes. To apply different render techniques to respective parts, we have to manually separate the meshes and hard code the render order for dependency concern. Job system provides handles to all sub-meshes for individual rendering jobs/steps by specified techniques.

The new mesh-hierarchy hence becomes Drawable -> Model (like the Sponza) -> Nodes (like a vase) -> Meshes (the vase drawable shared between all vases) -> Techniques (like the outline effect) -> Steps (the actual process to make the outline, which contains references to bindables and the mesh data). For each step, we register a job (contains ptr to mesh data and ptr to step/bindables).
Rendering

![Alt text](./Notes/12.jpg "Job Systen")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_FrameCommander.h">GRAPHICS_JOB_FrameCommander.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_FrameCommander.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A temporary solution for pass management. Will be replaced by a render graph later.</li>
	    		<li>Provides interfaces for submitting jobs to a specific pass. Responsible for executing passes.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Technique.h">GRAPHICS_JOB_Technique.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Technique.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Describes a specific type of technique. For example, phone shading.</li>
	    		<li>Contains an array of steps, which stores an array of bindables.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Step.h">GRAPHICS_JOB_Step.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Step.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Describes one of the steps to implement a technique.</li>
	    		<li>Contains an array of bindables necessary for this step. Bindables like ps and vs shaders are configured when initializing a step.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Job.h">GRAPHICS_JOB_Job.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Job.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Describes a job, which correlates to steps in every technique.</li>
	    		<li>Contains two pointers: 1) ptr to drawable data like vertecies. 2) ptr to step (bindables) like ps/vs shaders.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Pass.h">GRAPHICS_JOB_Pass.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_JOB_Pass.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Describes a render pass. Ideally, shared render processes like all phong shading meshes are packed in one pass.</li>
	    		<li>Contains an array of jobs (step bindables + drawable data).</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_NullPixelShader.h">GRAPHICS_OBJ_NullPixelShader.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_NullPixelShader.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A placeholer for techniques/steps do not need pixel shader. For example, the stencil-writing pass for the outline effect.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit and Files</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/TestCube.h">TestCube.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/b7354ae36f5c59543b86f3da385025b462856af7/DX11%203D%20Game%20Engine/TestCube.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>represents an outlined cube.</li>
	    		<li>Demonstrate how techniques/steps are configured and how passes/jobs are registered.</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/b7354ae36f5c59543b86f3da385025b462856af7">Job System</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Job system integrated into the old framework.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 16 - Dynamic shader constant system with shader layout pool and basic outline effect

![Alt text](./Screenshots/2021-12-26.png "Outline effect in scene")

![Alt text](./Notes/11.jpg "Dynamic constant system")

A system that examines model resources and automatically configures render procedures. For example, configure a alpha tester shader when alpha channel texture detected. This system also provides handles to all shader constants for run-time modification.

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/68554e945a52faf66827becfc2cbdcd4ca6d2296/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DynamicConstant.h">GRAPHICS_OBJ_DynamicConstant.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/68554e945a52faf66827becfc2cbdcd4ca6d2296/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DynamicConstant.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Implemented layout elements (leaf) and layout trees. These describes the essential data and procedure for a rendering style.</li>
	    		<li>The layout tree is capable of defining shader data recursively. For example, array of matrix of float4.</li>
	    		<li>Ultilized MACROS for leaf-node-types to reduce code redundancy (see <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/68554e945a52faf66827becfc2cbdcd4ca6d2296/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DynamicConstant.h">GRAPHICS_OBJ_DynamicConstant.h</a>).</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/68554e945a52faf66827becfc2cbdcd4ca6d2296/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_LayoutPool.h">GRAPHICS_OBJ_LayoutPool.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/68554e945a52faf66827becfc2cbdcd4ca6d2296/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_LayoutPool.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A layout pool system that allowing reusing registered layouts.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/68554e945a52faf66827becfc2cbdcd4ca6d2296/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Stencil.h">GRAPHICS_OBJ_Stencil.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/68554e945a52faf66827becfc2cbdcd4ca6d2296/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Stencil.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A bindable object that allows writing/referencing to the stencil buffer.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/68554e945a52faf66827becfc2cbdcd4ca6d2296">Dynamic constant layout system</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Dynamic constant layout system integrated.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/54fc2023b8910271fb4e12ee16d76feaa02d45fb">Stencil buffer</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Created outline effects for cubes bind with stencil objects.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 15 - [EXTRA] Cell Shading

![Alt text](./Screenshots/2021-12-12.png "Genshin Cell Shading")

### Demo Video [Youtube]

[![SC2 Video](https://img.youtube.com/vi/74ULjgIdN0E/0.jpg)](https://www.youtube.com/watch?v=74ULjgIdN0E)


## Commit 14 - Alpha testing and optional back-face culling

![Alt text](./Screenshots/2021-12-6.png "Alpha testing (class Blender) implemented")

![Alt text](./Screenshots/2021-12-7.png "Back-face culling disabler (class Rasterizer) implemented")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/54fc2023b8910271fb4e12ee16d76feaa02d45fb/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Blender.h">GRAPHICS_OBJ_Blender.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/54fc2023b8910271fb4e12ee16d76feaa02d45fb/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Blender.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A bindable object that could handle transparent textures (like a <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/54fc2023b8910271fb4e12ee16d76feaa02d45fb/DX11%203D%20Game%20Engine/models/Sponza/textures/vase_plant.png">flower texture</a>).</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/54fc2023b8910271fb4e12ee16d76feaa02d45fb/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Rasterizer.h">GRAPHICS_OBJ_Rasterizer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/54fc2023b8910271fb4e12ee16d76feaa02d45fb/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Rasterizer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A bindable object that could handle sheet-like models painted double-sided (like a <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/54fc2023b8910271fb4e12ee16d76feaa02d45fb/DX11%203D%20Game%20Engine/models/Sponza/textures/chain_texture.png">chain</a>). Back-face culling will be implicitly disabled for this case.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/54fc2023b8910271fb4e12ee16d76feaa02d45fb">Alpha testing through shaders.</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Also updated shader files to implement alpha test. For example, when sheet-model is rendered double-sided, the normal map z value should be flipped for the back face.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 13 - Mipmapping and Anisotropic Filtering with more models

![Alt text](./Screenshots/2021-11-15.png "Genshin Model imported")

![Alt text](./Screenshots/2021-11-16.png "Sponza Model imported")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/319085eb84140b84f8af2a7fe83e31942f9bc0e9/DX11%203D%20Game%20Engine/GRAPHICS_HELP_MatrixTranslator.h">GRAPHICS_HELP_MatrixTranslator.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/319085eb84140b84f8af2a7fe83e31942f9bc0e9/DX11%203D%20Game%20Engine/GRAPHICS_HELP_MatrixTranslator.cpp">cpp</a> <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/319085eb84140b84f8af2a7fe83e31942f9bc0e9/DX11%203D%20Game%20Engine/GRAPHICS_HELP_NormalMapTwerker.h">GRAPHICS_HELP_NormalMapTwerker.h</a> <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/319085eb84140b84f8af2a7fe83e31942f9bc0e9/DX11%203D%20Game%20Engine/GRAPHICS_HELP_TexturePreprocessor.h">GRAPHICS_HELP_TexturePreprocessor.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/319085eb84140b84f8af2a7fe83e31942f9bc0e9/DX11%203D%20Game%20Engine/GRAPHICS_HELP_TexturePreprocessor.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Several helper classes for adjusting imported art resources. For example, change from left-hand coordinate to the right hand one.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/319085eb84140b84f8af2a7fe83e31942f9bc0e9">Model loading</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Loaded models for Sponza and several Genshin Impact characters.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/65a6f9bd0d9ac3c26a06776f96d5ccc7d5cfcf27">Anisotropic filtering and mipmapping</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Configured automatic mipmapping and anisotropic filtering through DirectX interface.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 12 - Normal Mapping (Tangent Space)

![Alt text](./Notes/10.jpg "Normal Mapping")

![Alt text](./Screenshots/2021-11-13.png "Normal Mapping implemented")

![Alt text](./Screenshots/2021-11-13.gif "Normal Mapping in progress")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
		<td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/dd8fe88f921622ac21ec50a1c8f4d8322f717e38/DX11%203D%20Game%20Engine/PhongPSNormalMap.hlsl">PhongPSNormalMap.hlsl</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Shader file for basic normal Map</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/dd8fe88f921622ac21ec50a1c8f4d8322f717e38/DX11%203D%20Game%20Engine/TestPlane.h">TestPlane.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/dd8fe88f921622ac21ec50a1c8f4d8322f717e38/DX11%203D%20Game%20Engine/TestPlane.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A plane object that utilizes normal mapping.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/24f59e72020160d87953ba0ea979977aa73e4789"></a>Tangen space normal mapping</td>
	    <td align="left">
	    	<ul>
	    		<li>Updated shader file structures, refactored several shader functions, created several helper shader header files.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 11 - BindablePool system

![Alt text](./Notes/bindablepool_system.jpg "BindablePool system")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/eee52bd871b69c790ba5889b04867f3fcd0e24d9/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_BindablePool.h">GRAPHICS_OBJ_BindablePool.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/eee52bd871b69c790ba5889b04867f3fcd0e24d9/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_BindablePool.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Maintains the unordered_map< string, shared_ptr< Bindable > >.</li>
	    		<li>Using template programming/parameter pack to execute correct Bindable::Resolve<\Type>(param...).</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/ad93098637e6a134e861263e1a8ddf2cdcdf647a"></a>Model hierarchy loading</td>
	    <td align="left">
	    	<ul>
	    		<li>Implemented automatic/recursive hierarchy explorer that generates corresponding UI controllers for all model nodes - arms, legs, etc.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/31581d931d407df3dadfd4497df7dec5d30d508a">Mouse camera system</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Allows camera to follow mouse movements.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/d0efdcad7dc70b2e346407f0908bd5844a5953ed">Specular map loader</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Added specular constants loading codes in Mesh.cpp.</li>
	    		<li>Replaced StaticDrawInfo with shared_ptrs, ready for new BindablePool system.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/eee52bd871b69c790ba5889b04867f3fcd0e24d9">BindablePool system</a></td>
	    <td align="left">
	    	<ul>
	    		<li>A bindable pool system that matches bindable data with there id. Allows drawables to share bindables if necessary.</li>
	    		<li>Updated bindable base class such that all bindables need to implement resolve() to register themselves in bindable pool.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>



## Commit 10 - Model loading

![Alt text](./Screenshots/2021-9-16.png "Basic model loading")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/84ccba0d0e8c391d32f1b71297c0d3a094ebab9e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Mesh.h">GRAPHICS_OBJ_Mesh.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/84ccba0d0e8c391d32f1b71297c0d3a094ebab9e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Mesh.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Consists of Model, Mesh (drawable) and Node classes.</li>
	    		<li>Capable of reading model with filepath, parsing meshe/node-hierarchies and generating respective control window automatically (i.e. moving model's leg).</li>
	    		<li>Ultilized the dynamic vertex system, but haven't do flexible data reflection - detecting whether normal, diffuse, specular, alpha mask, etc. files exists and generate different dynamic vertex layouts accordingly.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/84ccba0d0e8c391d32f1b71297c0d3a094ebab9e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DynamicVertex.cpp">GRAPHICS_OBJ_DynamicVertex.cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/84ccba0d0e8c391d32f1b71297c0d3a094ebab9e/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_InputLayout.cpp">GRAPHICS_OBJ_InputLayout.cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Seperated definition and declaration.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Commit</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/84ccba0d0e8c391d32f1b71297c0d3a094ebab9e">Model loading</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Moved all rendering pipeline related objects into the GPipeline namespace.</li>
	    		<li>Imported assimp library.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 9 - DynamicVertex system

![Alt text](./Notes/7.jpg "DynamicVertex system")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/2bf0fe58981f9e4eb718897abf1902580f0e95b2/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_DynamicVertex.h">GRAPHICS_OBJ_DynamicVertex.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Provides an automatic vertex shader input layout/data manager. Previously the vertex data is defined manually in separate places (see duplicate codes in <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Box.cpp">GRAPHICS_OBJ_Box.cpp</a>).</li>
	    		<li>Now we only need to do VertexLayout.Append() definition ahead, and the layout object will be capable of reading vertex data, validating data and configures D3D layout and match shader automatically.</li>
	    		<li>Template programming and parameter pack are used to load vertex data flexibly (DynamicVertex::VertexBuffer::EmplaceBack()).</li>
	    		<li>This will be the code base for future improvements, including shader reflection.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/2bf0fe58981f9e4eb718897abf1902580f0e95b2/DX11%203D%20Game%20Engine/TexturedPhongVS.hlsl">TexturedPhongVS.hlsl</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2bf0fe58981f9e4eb718897abf1902580f0e95b2/DX11%203D%20Game%20Engine/TexturedPhongPS.hlsl">TexturedPhongPS.hlsl</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The phong shader capable of sampling texture.</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/2bf0fe58981f9e4eb718897abf1902580f0e95b2/DX11%203D%20Game%20Engine/AssTest.cpp.h">AssTest.cpp.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/2bf0fe58981f9e4eb718897abf1902580f0e95b2/DX11%203D%20Game%20Engine/AssTest.cpp.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Some test objects.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/2bf0fe58981f9e4eb718897abf1902580f0e95b2/DX11%203D%20Game%20Engine/GRAPHICS_BUF_VertexBuffer.h">GRAPHICS_BUF_VertexBuffer.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2bf0fe58981f9e4eb718897abf1902580f0e95b2/DX11%203D%20Game%20Engine/GRAPHICS_BUF_VertexBuffer.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Now capable of transporting a DynamicVertex object into the pipeline configurations.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 8 - Point light and phong shading

![Alt text](./Screenshots/2021-9-13.png "Specular")

![Alt text](./Screenshots/2021-9-13.gif "Specular")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/aac77b66377e3960c008439b3bdbf4d282269830/DX11%203D%20Game%20Engine/IndexedPhongVS.hlsl">IndexedPhongVS.hlsl</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/aac77b66377e3960c008439b3bdbf4d282269830/DX11%203D%20Game%20Engine/IndexedPhongPS.hlsl">IndexedPhongPS.hlsl</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Shaders responding to the new lighting settings and paint geometry faces with their respective face IDs.</li>
	    	</ul>
	    </td>
	</tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/aac77b66377e3960c008439b3bdbf4d282269830/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_TestObject.h">GRAPHICS_OBJ_TestObject.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/aac77b66377e3960c008439b3bdbf4d282269830/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Cylinder.h">GRAPHICS_OBJ_Cylinder.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/blob/aac77b66377e3960c008439b3bdbf4d282269830/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Cylinder.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Some test objects.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/commit/aac77b66377e3960c008439b3bdbf4d282269830">Commit details</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Transform buffer now stores model, model-view, model-view-proj matricies respectively for different shader calculations. For example, the normal calculation cannot be done after geometry being projected.</li>
	    		<li>Added slot parameter to transform and constant buffers for flexible registration.</li>
	    		<li>The pointlight now have various lighting attribute constants.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 7 - Point light and phong shading

![Alt text](./Notes/6.jpg "Phong shading")

![Alt text](./Screenshots/2021-9-12.png "Point light and phong shading")

![Alt text](./Screenshots/2021-9-12.gif "Point light and phong shading")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/GRAPHICS_LGT_PointLight.h">GRAPHICS_LGT_PointLight.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/GRAPHICS_LGT_PointLight.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Manages the light data attributes, control window and the Solidsphere object as visual representation.</li>
	    		<li>Updates the PixelConstantBuffer for shader's light computation.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_SolidSphere.h">GRAPHICS_OBJ_SolidSphere.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_SolidSphere.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Visualize the light source.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/PhongVS.hlsl">PhongVS.hlsl</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/PhongPS.hlsl">PhongPS.hlsl</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The Phong shaders.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_IndexedTriangleList.h">GRAPHICS_OBJ_IndexedTriangleList.h</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Added normal generator for cubes.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


## Commit 6 - Imgui and camara system

![Alt text](./Screenshots/2021-9-7.png "Imgui and camara system")

### New files

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/SYS_CLASS_ImguiManager.h">SYS_CLASS_ImguiManager.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/SYS_CLASS_ImguiManager.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Initialize Imgui system.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Camera.h">GRAPHICS_OBJ_Camera.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Camera.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Initialize camera positions, generate camera matrix using XMMatrixLookAtLH().</li>
	    		<li>Spawns Imgui control window that updates camera parameters.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

<table>
  <tbody>
    <tr>
      <th>Filename</th>
      <th align="center">Description</th>
    </tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/SYS_CLASS_Graphics.h">SYS_CLASS_Graphics.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/SYS_CLASS_Graphics.cpp">cpp</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/SYS_CLASS_App.h">SYS_CLASS_App.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/2d5b21530f2a1b29b27d918d19e10fa7531be5dc/DX11%203D%20Game%20Engine/SYS_CLASS_App.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Integrated Imgui frame into the current graphics frame system.</li>
	    		<li>Calls camera update.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

## Commit 5 - Texture mapping

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
	    		<li>A surface loader/modifier using Gdiplus. Convert image file into color data buffer.</li>
	    		<li>Contains a Color class providing various data handler into the bit data. Built corresponding Exception handler.</li>
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

### Major updates

None.


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
	    		<li>Various drawable geometries.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>

### Major updates

None.



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
	    		<li>Manages a collection of "bindables" objects that could be binded and shipped to rendering pipeline, such as vertex buffer, index buffer, transforms, shader constants and other topology settings.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Bindable.h">GRAPHICS_OBJ_Bindable.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/598c997abe43b5d43afa562fdff99eadd8b58f05/DX11%203D%20Game%20Engine/GRAPHICS_OBJ_Bindable.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>The base class (interface) for all bindable objects.</li>
	    		<li>The pure virtual function Bind(Graphics& gfx) expects different implementations from its children (like how vertex buffer and shader objects bind themselves to pipeline respectively).</li>
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

### Major updates

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


## Commit 2 - The first triangle with shaders

![Alt text](./Screenshots/2021-8-28.png "The first shape")

### New files

None.

### Major updates

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
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/f1642d0470232aeb334f447a8bd99ef40d53cfdc/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Mouse.h">SYS_CLASS_IO_Mouse.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/f1642d0470232aeb334f447a8bd99ef40d53cfdc/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Mouse.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Improved mouse moving-out-of-window behavior. The mouse tracker now stops when mouse is outside the window, like most modern games do.</li>
	    	</ul>
	    </td>
	</tr>
	<tr>
      <td><a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/f1642d0470232aeb334f447a8bd99ef40d53cfdc/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Keyboard.h">SYS_CLASS_IO_Keyboard.h</a> | <a href="https://github.com/CottageLord/DX11-3D-Game-Engine/tree/f1642d0470232aeb334f447a8bd99ef40d53cfdc/DX11%203D%20Game%20Engine/SYS_CLASS_IO_Keyboard.cpp">cpp</a></td>
	    <td align="left">
	    	<ul>
	    		<li>Distinguishes between text input and key pressing.</li>
	    	</ul>
	    </td>
	</tr>
  </tbody>
</table>


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

### Major updates

None.
