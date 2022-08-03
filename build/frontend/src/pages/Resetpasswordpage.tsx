import React, { useState } from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';

import StyledInput from '../components/StyledInput';
import Button from '../components/ReusableButton';
import {Routes, Route, useNavigate, useParams} from 'react-router-dom';
import { getStoredUID, useAuth } from '../global/GlobalAuth';
import YavalathButton from '../components/YavalathButton';
import { useAlert } from '../global/GlobalAlert';
import { resetPassword } from '../api/rest';
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
  const [oldpass, setOldPass] = useState<string>("");
  const [newpass, setNewPass] = useState<string>("");
  const { setError, setInfo } = useAlert();

  // Get iser info 
  const { getUID } = useAuth();
  const uid = window.localStorage.getItem("username")
  
  const navigate = useNavigate();
  const navigateToDashboard = () => {
    navigate('/dashboard');
  };
  
  const handleClick = async () => {
    //setInfo('Registering user, please wait', 5000);
    const uid = String(getStoredUID())
    const response = await resetPassword(uid, oldpass, newpass)
    const { outcome, message } = response.payload
    if(outcome == 'success') {
      setInfo("Your password has been changed!", 5000);
      navigateToDashboard();
    } else {
      console.log('register error');
      setError(message)
    }
  }
  
  return (      
    <Container>
      <YavalathButton/>
      <Typography variant="h3">Reset Password</Typography>
      <Typography variant="h4">Hi {uid}</Typography>

      <StyledInput onChange={(e) => {setOldPass(e.currentTarget.value)}} value={oldpass} label="old password"/>
      <StyledInput onChange={(e) => {setNewPass(e.currentTarget.value)}} value={newpass} label="new password"/>
      <Button onClick={handleClick}> Submit </Button>
    </Container>
  )
}