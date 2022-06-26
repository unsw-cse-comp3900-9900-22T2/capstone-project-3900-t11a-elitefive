import React from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';

import StyledInput from '../components/StyledInput';
import Button from '../components/ReusableButton';

type Props = {}

const Container = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export default function Loginpage({}: Props) {
  return (
    <Container>
      <Typography variant="h3">Login</Typography>
      <StyledInput label="username"/>
      <StyledInput label="password" password/>
      <Button label="Login"></Button>
    </Container>
  )
}