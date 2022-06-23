import React from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';

import StyledInput from '../components/StyledInput';

type Props = {}

const Container = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export default function Registerpage({}: Props) {
  return (
    <Container>
      <Typography variant="h3">Register</Typography>
      <StyledInput label="email"/>
      <StyledInput label="username"/>
      <StyledInput label="password" password/>
      {/** to replace button with styled button */}
      <button>register</button>
    </Container>
  )
}