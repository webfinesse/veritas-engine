//
//  MatrixStack.cpp
//  Test
//
//  Created by Brett Knapik on 2/14/15.
//  Copyright (c) 2015 Brett Knapik. All rights reserved.
//

#include <stack>
#include "MatrixStack.h"

struct VeritasEngine::MatrixStack::Impl : public VeritasEngine::SmallObject<>
{
public:
    Impl()
    : m_stack{}
    {
        
    }

    std::stack<Matrix4x4> m_stack;
};

VeritasEngine::MatrixStack::MatrixStack()
	: m_impl{ std::make_unique<Impl>() }
{    
	Matrix4x4 identitySimd = Matrix4x4();
	m_impl->m_stack.push(identitySimd);
}

VeritasEngine::MatrixStack::~MatrixStack() = default;

void VeritasEngine::MatrixStack::Push(const Matrix4x4& matrix)
{
	auto right = m_impl->m_stack.top();
    
	Matrix4x4 result = matrix * right;
    
	m_impl->m_stack.push(result);
}

const VeritasEngine::Matrix4x4 & VeritasEngine::MatrixStack::Peek() const
{
    return m_impl->m_stack.top();
}

void VeritasEngine::MatrixStack::Pop()
{
    if(m_impl->m_stack.size() > 1)
    {
        m_impl->m_stack.pop();
    }
}