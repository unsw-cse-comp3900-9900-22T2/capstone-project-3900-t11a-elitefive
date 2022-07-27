import React from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import YavalathButton from '../components/YavalathButton';
import ProfileWidget from '../components/ProfileWidget';
import ScrollWidget from '../components/ScrollWidget';

type Props = {}

const Container  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: Column;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

const Container1  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

export default function Dashboard({}: Props) {

  const navigate = useNavigate();

  const navigateToGamepage = () => {
      // 👇️ navigate to /contacts
      navigate('/gamemode');
  };
  const navigateToReplays = () => {
    // 👇️ navigate to /contacts
    navigate('/replays');
  };

  const navigateToLeaderboard = () => {
    // 👇️ navigate to /contacts
    navigate('/leaderboard');
  };

  return (
    <Container>
      <ProfileWidget/>
      <Container1>
        <LargeButton onClick={navigateToGamepage}>
          <Typography variant="h3">{"Play"}</Typography>
        </LargeButton>
        <LargeButton onClick={navigateToLeaderboard}>
          <Typography variant="h3">{"Leaderboard"}</Typography>
        </LargeButton>
        <LargeButton onClick={navigateToReplays}>
          <Typography variant="h3">{"Replays"}</Typography>
        </LargeButton>
      </Container1>
      <ScrollWidget variant="bottom"/>
    </Container>
  )
}