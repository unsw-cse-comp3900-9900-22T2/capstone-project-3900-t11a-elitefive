import React from 'react'
import styled from 'styled-components';

type Props = {
  children?: any;
}

// makes all pages have the same background color
const Container = styled.div`
  background: var(--background-color);
  color: var(--text-primary);
  width: 100vw;
  min-height: 100vh;
`;

export default function PageContainer({ children }: Props) {
  return (
    <Container>
      {children}
    </Container>
  )
}