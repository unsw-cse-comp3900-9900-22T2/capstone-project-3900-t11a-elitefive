import React from 'react'
import styled from 'styled-components';

type Props = {}

const Container = styled.div`
  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
`;

export default function Registerpage({}: Props) {
  return (
    <Container>
      {/* <p>Register</p> */}
      <div>Register</div>
      

    </Container>
  )
}