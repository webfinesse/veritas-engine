//
//  MatrixStack.cpp
//  Test
//
//  Created by Brett Knapik on 2/14/15.
//  Copyright (c) 2015 Brett Knapik. All rights reserved.
//

#include <vector>
#include "MatrixStack.h"

struct VeritasEngine::MatrixStack::Impl : public VeritasEngine::SmallObject<>
{
public:
    Impl()
    : m_stack{}
    {
		m_stack.reserve(10);
    }

    std::vector<Matrix4x4> m_stack;
};

VeritasEngine::MatrixStack::MatrixStack()
	: m_impl{ std::make_unique<Impl>() }
{    
	m_impl->m_stack.emplace_back();
}

VeritasEngine::MatrixStack::~MatrixStack() = default;

void VeritasEngine::MatrixStack::Push(const Matrix4x4& matrix)
{
	m_impl->m_stack.emplace_back(m_impl->m_stack.back() * matrix);
}

const VeritasEngine::Matrix4x4 & VeritasEngine::MatrixStack::Peek() const
{
    return m_impl->m_stack.back();
}

void VeritasEngine::MatrixStack::Pop()
{
    if(m_impl->m_stack.size() > 1)
    {
        m_impl->m_stack.pop_back();
    }
}