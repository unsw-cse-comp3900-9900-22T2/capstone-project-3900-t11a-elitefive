import React, { useEffect, useState } from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography, Box } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate, useParams} from 'react-router-dom';
import YavalathButton, {YavalathButtonFixed} from '../components/YavalathButton';
import ProfileWidget, {ProfileCard} from '../components/ProfileWidget';
import ScrollWidget from '../components/ScrollWidget';
import ButtonGroup from '@mui/material/ButtonGroup';
import ReusableToggleButtonGroup from '../components/ReusableToggleButtonGroup';
import ReusableToggleButton from '../components/ReusableToggleButton';
import ToggleButton from '@mui/material/ToggleButton';
import {Card2, Card3, statCard, MatchCard} from '../components/ReusableCard';
import CustomizedInputBase from '../components/SearchBar';
import Tabs from '@mui/material/Tabs';
import Tab from '@mui/material/Tab';
import StatTab from '../components/Tab';

type Props = {}

type BoxContainerProps = {
  minHeight?: number;
}

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
const StatContainer  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 0px;
`

const MatchContainer  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: column;

  justify-content: center;
  align-items: center;
  grid-gap: 10px;
`

export type profileDataType = {
  uid: number;
  username: string;
  email: string;
  classic: statsDataType;
  potholes: statsDataType;
  triples: statsDataType;
}

type statsDataType = {
  draws: number;
  elo: number;
  losses: number;
  wins: number;
}

const DefaultProfileData = {
  "email": "",
  "username": "",
  "uid": 0,
  "classic": {
    "draws": 0,
    "elo": 0,
    "losses": 0,
    "wins": 0
  },
  "potholes": {
    "draws": 0,
    "elo": 0,
    "losses": 0,
    "wins": 0
  },
  "triples": {
    "draws": 0,
    "elo": 0,
    "losses": 0,
    "wins": 0
  },
}





export default function Profilepage({}: Props) {
  const [profileData, setProfileData] = useState<profileDataType>(DefaultProfileData);
  const navigate = useNavigate();

  const { uid } = useParams();

  const navigateToFriends = () => {
      // 👇️ navigate to /contacts
      navigate('/friends');
  };

  useEffect(() => {
    fetch(`/api/profile?uid=${uid}`)
    .then(resp => resp.json())
    .then(data => {
      const payload = data.payload;
      setProfileData(payload);
    })
  },[])


  return (
    <Container>
      <YavalathButtonFixed/>
      <Container1>
        <Container>
          <ProfileCard name={profileData.username}/>
          <CustomizedInputBase/>
          <Button align-items="right" onClick={navigateToFriends}>Friends</Button>
        </Container>
        <StatContainer>
          <StatTab data={profileData}/>
        </StatContainer>
      </Container1>
      <Typography variant="h3">{"Past Matches"}</Typography>
      <MatchContainer>
        <MatchCard/>
        <MatchCard/>
        <MatchCard/>
      </MatchContainer>
    </Container>
  )
}