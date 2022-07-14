import React, { useState } from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';

import StyledInput from '../components/StyledInput';
import Button from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import {register} from '../api/rest'

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
  const [email, setEmail] = useState<string>("");
  const [username, setUsername] = useState<string>("");
  const [password, setPassword] = useState<string>("");

  const navigate = useNavigate();
  
  const navigateToDashboard = () => {
    // 👇️ navigate to /contacts
    navigate('/dashboard');
  };
  
  const handleClick = async () => {
    const success = await register(username, password, email)
    if(success) {
      navigate("/login");
    } else {
      console.log('register error');
    }
  }

  return (
    <Container>
      <Typography variant="h3">Register</Typography>
      <StyledInput onChange={(e) => {setEmail(e.currentTarget.value)}} value={email} label="email"/>
      <StyledInput onChange={(e) => {setUsername(e.currentTarget.value)}} value={username} label="username"/>
      <StyledInput onChange={(e) => {setPassword(e.currentTarget.value)}} value={password} label="password"/>
      <Button onClick={handleClick}> Submit </Button>
    </Container>
  )
}