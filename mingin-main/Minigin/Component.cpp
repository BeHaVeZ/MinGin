#include "pch.h"

dae::Component::Component():
	m_Subject{new Subject()}
{
}
dae::Component::~Component()
{
	delete m_Subject;
	m_Subject = nullptr;
}
