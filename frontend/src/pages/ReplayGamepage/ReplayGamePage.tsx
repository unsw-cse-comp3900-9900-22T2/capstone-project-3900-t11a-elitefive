import React, { useEffect, useRef, useState } from 'react'
import styled from 'styled-components';

// local
import { GSProvider, useGameState } from '../../global/GlobalGameState';
import { WSProvider } from '../../global/GlobalWS';
import Board from '../../components/Board';
import YavalathButton, { YavalathButtonFixed } from '../../components/YavalathButton';

import Sidebar from './ReplaySidebar';
import { useParams } from 'react-router-dom';
import { Typography } from '@mui/material';

type Props = {}

const Container = styled.div`
`;

const Container2 = styled.div`
  display: flex;
  flex-direction: row;

  justify-content: center;
  align-items: center;
  grid-gap: 30px;
`;

export type matchDataType = {
  gamemode: string;
  holes: string;
  match_id: number;
  move_seq: Array<Array<string>>;
  moves: string;
  nplayers: number;
  players: Array<playersDataType>;
}

type playersDataType = {
  elo_end: number;
  elo_start: number;
  outcome: string;
  uid: number;
  username: string;
}

const DefaultMatchData = {
    "gamemode": "",
    "holes": "",
    "match_id": 0,
    "move_seq": [
        [
            ""
        ],
        [
            ""
        ]
    ],
    "moves": "",
    "nplayers": 0,
    "players": [
        {
            "elo_end": 0,
            "elo_start": 0,
            "outcome": "",
            "uid": 0,
            "username": ""
        },
        {
            "elo_end": 0,
            "elo_start": 0,
            "outcome": "",
            "uid": 0,
            "username": ""
        }
    ],
}

const DefaultGamemode = {
  "gamemode" : "",
}

export type gamemodeDataType = {
  gamemode: string;
}


export default function ReplayGamepage({}: Props) {

  const [matchData, setMatchData] = useState<matchDataType>(DefaultMatchData);
  const [gamemode, setGamemode] = useState<gamemodeDataType>(DefaultGamemode);
  const{ matchid } = useParams();
  const { setWinner } = useGameState();

  useEffect(() => {
    fetch(`/api/replay?matchid=${matchid}`)
    .then(resp => resp.json())
    .then(data => {
      setGamemode(data.gamemode);
      setMatchData(data);
    })
  },[])

  return (
         <Container>
           <YavalathButtonFixed/>
           <Container2>
              <Board/>
              <Sidebar/>
           </Container2>
         </Container>
  )
}