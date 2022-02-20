#pragma once

class IRenderable
{
	virtual void Render(class Shader* shader) = 0;
};