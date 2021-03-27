#pragma once
#ifndef _LINE_H_
#define _LINE_H_

#include <glm/vec2.hpp>

struct Line
{
public:
	Line() {}
	void setLine(glm::vec2 s, glm::vec2 e)
	{
		start = s;
		end = e;
	}
	glm::vec2 Start() const { return start; }
	glm::vec2 End() const { return end; }
	friend class PlayScene;
private:
	glm::vec2 start;
	glm::vec2 end;
};

#endif // !_LINE_H_
#define _LINE_H_