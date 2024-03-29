import React from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Box, Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import YavalathButton from '../components/YavalathButton';
import ProfileWidget from '../components/ProfileWidget';
import ScrollWidget from '../components/ScrollWidget';
import Feedpage from './Feedpage';

import Cube from '../assets/Cube.svg';
import Leaderboards from '../assets/Leaderboards.png';
import Replays from '../assets/Replays.png';

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
  const navigateToInviteFriends = () => {
    navigate('/invite');
  }

  return (
    <Container>
      <ProfileWidget/>
      <Container1>
        <Box display="flex" flexDirection="column">
          <LargeButton onClick={navigateToGamepage} background="var(--accent-purple)" height={350}>
            <Box display="flex" justifyContent="center">
              <img src={Cube} style={{objectFit:"cover"}}/>
            </Box>
            <Typography variant="h3">Play</Typography>
          </LargeButton>
          <Button 
            background="var(--accent-blue)" 
            onClick={navigateToInviteFriends}
            width={350}
            height={50}
          >Play with Friends</Button>
        </Box>
        <LargeButton onClick={navigateToLeaderboard}>
          <Box width="100%" height="150px" display="flex" alignItems="center" justifyContent="center">
            <img src={Leaderboards} style={{objectFit: "cover"}}/>
          </Box>
          <Typography variant="h3">{"Leaderboard"}</Typography>
        </LargeButton>
        <LargeButton onClick={navigateToReplays}>
          <Box width="100%" height="150px" display="flex" alignItems="center" justifyContent="center">
            <img src={Replays}/>
          </Box>
          <Typography variant="h3">{"Replays"}</Typography>
        </LargeButton>
      </Container1>
      <ScrollWidget variant="bottom"/>
      <Feedpage/>
    </Container>
  )
}