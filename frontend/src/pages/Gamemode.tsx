import React, { useEffect, useRef, useState } from 'react';
import styled from 'styled-components';
import Board from '../components/Board';
import { Box, Modal, Slider, Typography } from '@mui/material';
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
import PopupContainer from '../components/PopupContainer';

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

const SliderContainer = styled.div`
  width: 300px;
  color: white;
`;

export default function Gamemode({}: Props) {

    const navigate = useNavigate();

    const navigateToGamepage = () => {
        // 👇️ navigate to /contacts
        navigate('/game');
    };

    const [waitingRoomSock, setWaitingRoomSock] = useState<WebSocket|undefined>();
    const [vsAI, setVsAI] = useState(true);
    const [isRanked, setIsRanked] = useState(true);
    const [isWaiting, setIsWaiting] = useState(false);
    const [aiDifficulty, setAIDifficulty] = useState<number|number[]>(0);

    const { getUID } = useAuth();
    
    const handleJoinWaitingRoom = (gamemode:string) => async () => {
      if(!waitingRoomSock) {
        const uid = getUID();
        // console.log(uid);
        if(!uid) return;
        const ws = await JoinSocket(parseInt(uid));
        setIsWaiting(true);


        const aiDiff = (aiDifficulty > 50) ? 2 :
                        (
                          (aiDifficulty < 50) ? 0 : 1
                        )
                        
        console.log(`aidiff = ${aiDiff}`);

        setTimeout(() => {
          ws.send(JSON.stringify({
            "data": JSON.stringify({
              "uid": uid,
              "ranked": isRanked,
              "ai": vsAI,
              "gamemode": gamemode,
              "ai-difficulty": aiDiff
            })
          }));
        },1000)

        setWaitingRoomSock(ws);
      }
    }

    const renderStepTwo = () => {
      if(vsAI) {
        const handleSlider = (event: Event, value:number|number[]) => {
          setAIDifficulty(value);
        }

        return (
          <SliderContainer>
            <Typography variant="h4">AI Difficulty</Typography>
            <Slider
              value={aiDifficulty}
              step={50}
              sx={{
                color: "white",
              }}
              onChange={handleSlider}
             />
          </SliderContainer>
        )
      }
      // else
      return (
        <Box display="flex">
          <Button
            onClick={() => {setIsRanked(true)}}
            background={isRanked ? "var(--accent-darker)" : "var(--accent-dark)"}
          >
            Ranked
          </Button>
          <Button 
            onClick={() => {setIsRanked(false)}}
            background={!isRanked ? "var(--accent-darker)" : "var(--accent-dark)"}
          >
            casual
          </Button>
        </Box>
      )

    }

    useEffect(() => {
      console.log(aiDifficulty)
    },[aiDifficulty])

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
          <Modal open={isWaiting} onClose={() => {} }>
            <PopupContainer>
              <Box height="100%" display="flex" justifyContent="center" alignItems="center">
              <Typography variant="h5">waiting for other players...</Typography>
              </Box>
            </PopupContainer>
          </Modal>
          <Typography variant="h3">{"Select Game Mode"}</Typography>
          <Box display="flex">
            <Button 
              onClick={() => {setVsAI(true)}}
              background={vsAI ? "var(--accent-darker)" : "var(--accent-dark)"}
            >
              Vs AI
            </Button>
            <Button 
              onClick={() => {setVsAI(false)}}
              background={!vsAI ? "var(--accent-darker)" : "var(--accent-dark)"}
            >
              Vs Human
            </Button>
          </Box>
          {renderStepTwo()}
          <Container1>
            <LargeButton onClick={handleJoinWaitingRoom("CLASSIC")}>
              <Typography variant="h3">{"Classic"}</Typography>
            </LargeButton>
            <LargeButton onClick={handleJoinWaitingRoom("TRIPLES")}>
              <Typography variant="h3">{"Triples"}</Typography>
            </LargeButton>
            <LargeButton onClick={handleJoinWaitingRoom("POTHOLES")}>
              <Typography variant="h3">{"Pothole"}</Typography>
            </LargeButton>
          </Container1>
        </Container>
      )
}