import React, { useEffect, useState } from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate, useParams} from 'react-router-dom';
import YavalathButton, { YavalathButtonFixed } from '../components/YavalathButton';
import ProfileWidget from '../components/ProfileWidget';
import ScrollWidget from '../components/ScrollWidget';
import { LeaderTab } from '../components/Tab';
import { Box } from '@mui/system';
import { useAuth } from '../global/GlobalAuth';
import { getuid } from 'process';

type Props = {}

const Container  = styled.div`
  min-height: 100vh;

  display: flex;
  flex-direction: Column;
  
  justify-content: top;
  align-items: center;
  grid-gap: 30px;
`

const Container1  = styled.div`
  min-height: 50vh;

  display: flex;
  flex-direction: column;

  margin-top: 100px
  justify-content: top;
  align-items: center;
  grid-gap: 30px;
`

export type leaderDataType = {
  classic: Array<boardDataType>;
  potholes: Array<boardDataType>;
  triples: Array<boardDataType>;
}

type boardDataType = {
  draws: number;
  elo?: number;
  losses: number;
  rank: number;
  uid: number;
  username: string;
  wins: number;
}

const DefaultLeaderData = {
  "classic": [{
    "draws": 0,
    "elo": 1000,
    "losses": 0,
    "rank": 0,
    "uid": 0,
    "username": "",
    "wins": 0,
  }],
  "potholes": [{
    "draws": 0,
    "elo": 1000,
    "losses": 0,
    "rank": 0,
    "uid": 0,
    "username": "",
    "wins": 0,
  }],
  "triples": [{
    "draws": 0,
    "elo": 1000,
    "losses": 0,
    "rank": 0,
    "uid": 0,
    "username": "",
    "wins": 0,
  }]
}

export default function Leaderboard({}: Props) {
  const [friendData, setFriendData] = useState<leaderDataType>(DefaultLeaderData);
  const [globalData, setGlobalData] = useState<leaderDataType>(DefaultLeaderData);

  const { getUID } = useAuth();

  useEffect(() => {
    fetch(`/api/leaderboards?uid=${getUID()}`)
    .then(resp => resp.json())
    .then(data => {
      console.log(data);
      if (!data) return;
      const friendload = data["friend_leaderboard"];
      const globalload = data["global_leaderboard"];
      setFriendData(friendload);
      setGlobalData(globalload);
    })
  }, [])

  const [buttonState, setButtonState] = useState(false);

  function toggleStatus() {
    setButtonState(!buttonState);
  }

  function dataType() {
    if (!buttonState) {
      return <LeaderTab data={friendData}/>
    } else {
      return <LeaderTab data={globalData}/>
    }
  }
  

  return (
    
    <Container>
      <YavalathButtonFixed/>
      <ProfileWidget/>
      <Container1>
        <Box margin="80px 50px">
          <Box>
            <Button onClick={() => toggleStatus()}>
              {buttonState ? "Toggle Friend" : "Toggle Global"}
            </Button>
          </Box>
          {/* {dataType()} */}
          <LeaderTab data={friendData}/>
        </Box>
      </Container1>
    </Container>
  )
}