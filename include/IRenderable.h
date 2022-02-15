#pragma once

class IRenderable
{
	virtual void PreRender() = 0;
	virtual void Render() = 0;
};