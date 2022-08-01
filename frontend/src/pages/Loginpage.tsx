import React, { useState } from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';

import StyledInput from '../components/StyledInput';
import Button from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import { useAuth } from '../global/GlobalAuth';
import YavalathButton from '../components/YavalathButton';
import { useAlert } from '../global/GlobalAlert';


type Props = {}

const Container = styled.div`
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

const StyledLink = styled.a`
  font-size: 20px;

`;

export default function Loginpage({}: Props) {
  const [email, setEmail] = useState<string>("");
  const [password, setPassword] = useState<string>("");
  const { login } = useAuth();
  const { setError } = useAlert();
  const navigate = useNavigate();

  const navigateToDashboard = () => {
      // 👇️ navigate to /contacts
      navigate('/dashboard');
  };
  
  const handleClick = async () => {
    try {
      const result = await login(email,password)
      if(result) {
        navigateToDashboard();
      }
    } catch (err) {
      console.log('invalid credentials');
      setError('invalid credentials');
      return;
    }
  }
  
  const handleForgotPw = async () => {
    console.log("forgot password")
    navigate("forgotpassword")
  }

  return (
    <Container>
      <YavalathButton/>
      <Typography variant="h3">Login</Typography>
      <StyledInput onChange={(e) => {setEmail(e.currentTarget.value)}} value={email} label="email"/>
      <StyledInput onChange={(e) => {setPassword(e.currentTarget.value)}} value={password} label="password" password/>
      <Button onClick={handleClick}> Submit </Button>
      {/* <Button onClick={handleForgotPw}> Forgot Password </Button> */}
      <StyledLink href="/login/forgotpassword">forgot password</StyledLink>
    </Container>
  )
}