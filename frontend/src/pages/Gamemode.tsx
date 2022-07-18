import React, { useEffect, useRef, useState } from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Typography } from '@mui/material';
import StyledInput from '../components/StyledInput';
import {StyledButton} from '../components/ReusableButton-styled';
import Button, {Button2, LargeButton} from '../components/ReusableButton';
import {Routes, Route, useNavigate} from 'react-router-dom';
import YavalathButton, { YavalathButtonFixed } from '../components/YavalathButton';
import ProfileWidget from '../components/ProfileWidget';
import ScrollWidget from '../components/ScrollWidget';
import ButtonGroup from '@mui/material/ButtonGroup';
import ReusableToggleButtonGroup from '../components/ReusableToggleButtonGroup';
import ReusableToggleButton from '../components/ReusableToggleButton';
import ToggleButton from '@mui/material/ToggleButton';
import { useAuth } from '../global/GlobalAuth';

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
  min-height: 50vh;

  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`

export default function Gamemode({}: Props) {

    const navigate = useNavigate();

    const navigateToGamepage = () => {
        // 👇️ navigate to /contacts
        navigate('/game');
    };

    const [waitingRoomSock, setWaitingRoomSock] = useState<WebSocket|undefined>();
    const [vsAI, setVsAI] = useState(false);
    const [isRanked, setIsRanked] = useState(false);

    const { getUID } = useAuth();
    
    const handleJoinWaitingRoom = async () => {
      if(!waitingRoomSock) {
        const uid = getUID();
        console.log(uid);
        if(!uid) return;
        const ws = await JoinSocket(parseInt(uid));

        setTimeout(() => {
          ws.send(JSON.stringify({
            "data": JSON.stringify({
              "uid": uid,
              "ranked": vsAI,
              "ai": isRanked,
            })
          }));
        },1000)

        setWaitingRoomSock(ws);
      }
    }

    const JoinSocket = (uid: number): Promise<WebSocket> => {
      return new Promise((resolve, reject) => {
        type Payload = {
          event: string;
          room_id: number;
          uids: number[];
        }
        const ws: WebSocket = new WebSocket('ws://localhost:8080/ws/waitingroom');
        ws.onopen = () => {
          console.log('HELLO');
        }
        // event listeners
        ws.onmessage = (message) => {
          // what I am expecting to receive
          const payload = JSON.parse(message.data) as Payload;
          console.log(payload);
          switch(payload.event) {
            case "match_created":{
              const { room_id , uids } = payload;
              if(uids.includes(uid)) {
                ws.close();
                navigate(`/game/${room_id}`);
              }
            }
          }
        }
        resolve(ws);
      })
    }

    useEffect(() => {
      // setTimeout(() => {
      //   waitingRoomSock?.close();
      // }, 5000);
    })

    useEffect(() => {
      console.log(waitingRoomSock);
    },[waitingRoomSock])


    return (
        <Container>
          <YavalathButtonFixed/>
          <ProfileWidget/>
          <Typography variant="h3">{"Select Game Mode"}</Typography>
          <ReusableToggleButtonGroup>
            <ToggleButton value="Vs Ai" onClick={() => {setVsAI(true)}}>Vs Ai</ToggleButton>
            <ToggleButton value="Vs Human" onClick={() => {setVsAI(false)}}>Vs Human</ToggleButton>
          </ReusableToggleButtonGroup>
          <ReusableToggleButtonGroup>
            <ToggleButton value="Ranked" onClick={() => {setIsRanked(true)}}>Ranked</ToggleButton>
            <ToggleButton value="Casual" onClick={() => {setIsRanked(false)}}>Casual</ToggleButton>
          </ReusableToggleButtonGroup>
          <Container1>
            <LargeButton onClick={handleJoinWaitingRoom}>
              <Typography variant="h3">{"Classic"}</Typography>
            </LargeButton>
            <LargeButton onClick={handleJoinWaitingRoom}>
              <Typography variant="h3">{"Triplets"}</Typography>
            </LargeButton>
            <LargeButton onClick={handleJoinWaitingRoom}>
              <Typography variant="h3">{"Pothole"}</Typography>
            </LargeButton>
          </Container1>
        </Container>
      )
}