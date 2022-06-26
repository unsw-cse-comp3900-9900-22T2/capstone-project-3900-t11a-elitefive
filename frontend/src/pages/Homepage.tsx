import React from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button from '../components/ReusableButton';

type Props = {}

const Container  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

const Container1 = styled.div`
min-height: 120vh;

display: flex;
flex-direction: column;

justify-content: center;
align-items: center;
grid-gap: 30px;
`

const Container2  = styled.div`
  min-height: 80px;

  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

export default function Homepage({}: Props) {
  return (
    <Container>
      <Board/>
      <Container1>
        <Typography variant="h1">Yavalath</Typography>
        <Container2>
          <Button label="Login"></Button>
          <Button label="Register"></Button>
        </Container2>
        <Button label="Play Now"></Button>
      </Container1>
      
    </Container> 
  )
}