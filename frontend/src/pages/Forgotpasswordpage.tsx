import React, { useState } from 'react'
import styled from 'styled-components';
import { Typography } from '@mui/material';

import StyledInput from '../components/StyledInput';
import Button from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import { useAuth } from '../global/GlobalAuth';
import YavalathButton from '../components/YavalathButton';
import { useAlert } from '../global/GlobalAlert';
import { forgotPassword } from '../api/rest';
import { sortedLastIndexOf } from 'lodash';


type Props = {}

const Container = styled.div`
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export default function Forgotpasswordpage({}: Props) {
  const [email, setEmail] = useState<string>("");
  const { setError, setInfo } = useAlert();
  
  const handleClick = async () => {
    try {
        const result = await forgotPassword(email)
        const { outcome, message } = result.payload
        if (outcome =="failure"){
            setError(message)
        } 
        else{
            setInfo("Check email for your new password", 5000)
        } 
      } catch (err) {
        console.log('an error occured');
        setError('an error occured');
        return;
      }
  }
  
  return (      
    <Container>
      <YavalathButton/>
      <Typography variant="h3">Forgot Password</Typography>
      <Typography variant="h4">Enter your email and a temporary password will be sent</Typography>

      <StyledInput onChange={(e) => {setEmail(e.currentTarget.value)}} value={email} label="email"/>
      <Button onClick={handleClick}> Submit </Button>
    </Container>
  )
}