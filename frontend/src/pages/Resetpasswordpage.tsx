import React, { useState } from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';

import StyledInput from '../components/StyledInput';
import Button from '../components/ReusableButton';
import {Routes, Route, useNavigate, useParams} from 'react-router-dom';
import { useAuth } from '../global/GlobalAuth';
import YavalathButton from '../components/YavalathButton';
import { useAlert } from '../global/GlobalAlert';
import { forgotPassword } from '../api/rest';
import { sortedLastIndexOf } from 'lodash';
import { convertCompilerOptionsFromJson, createImportSpecifier } from 'typescript';


type Props = {}

const Container = styled.div`
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export default function Resetpasswordpage({}: Props) {
  const [email, setEmail] = useState<string>("");
  const [password, setPassword] = useState<string>("");
  const { login } = useAuth();
  const { setError, setInfo } = useAlert();
  const navigate = useNavigate();
  
  
  // Get iser info 
  const { getUID } = useAuth();
  const uid = window.localStorage.getItem("username")
  
  const handleClick = async () => {
    console.log(window.localStorage)
  }
  
  return (      
    <Container>
      <YavalathButton/>
      <Typography variant="h3">Reset Password</Typography>
      <Typography variant="h4">hi {uid} </Typography>

      <StyledInput onChange={(e) => {setEmail(e.currentTarget.value)}} value={email} label="old password"/>
      <StyledInput onChange={(e) => {setEmail(e.currentTarget.value)}} value={email} label="new password"/>
      <Button onClick={handleClick}> Submit </Button>
    </Container>
  )
}