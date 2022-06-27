import React from 'react'
import styled from 'styled-components';

type Props = {
  children: any;
}

const Container = styled.div`
  width: 700px;
  height: 500px;
  background: var(--accent-darker);
  border-radius: 10px;

  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
`;

export default function PopupContainer({ children }: Props) {
  return (
    <Container>
      {children}
    </Container>
  )
}