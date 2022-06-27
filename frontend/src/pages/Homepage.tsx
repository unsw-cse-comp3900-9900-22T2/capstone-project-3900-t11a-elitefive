import React from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2} from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';

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
grid-gap: 15px;
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
  const navigate = useNavigate();

  const navigateToLogin = () => {
    // 👇️ navigate to /contacts
    navigate('/login');
  };

  const navigateToRegister = () => {
    // 👇️ navigate to /contacts
    navigate('/register');
  };

  const navigateToDashboard = () => {
    // 👇️ navigate to /contacts
    navigate('/dashboard');
  };

  return (
    <Container>
      <Board/>
      <Container1>
        <Typography variant="h1">Yavalath</Typography>
        <Container2>
          <Button onClick={navigateToLogin}>Login</Button>
          <Button onClick={navigateToRegister}>Register</Button>
        </Container2>
        <Button2 width={270} height={100} onClick={navigateToDashboard}>
          <Typography variant="caption">{"Already logged in?"}</Typography>
          <Typography variant="h4">{"Play Now!"}</Typography>
        </Button2>
      </Container1>
      
    </Container> 
  )
}