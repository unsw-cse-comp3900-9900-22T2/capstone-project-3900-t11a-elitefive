import React, { useEffect, useRef, useState } from 'react'
import styled from 'styled-components';
import { useParams } from 'react-router-dom';

// local
import { GSProvider, useGameState } from '../../global/GlobalGameState';
import { WSProvider } from '../../global/GlobalWS';
import Board from '../../components/Board';
import YavalathButton from '../../components/YavalathButton';

import PopupStatefulComponent from './PopupStatefulComponent';
import Sidebar from './Sidebar';

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



export default function Gamepage({}: Props) {
  const { gameid } = useParams();
  return (
    <GSProvider>
      <WSProvider gameId={gameid}>
        <Container>
          <YavalathButton/>
          <Container2>
            <Board />
            <Sidebar />
          </Container2>
          <PopupStatefulComponent/>
        </Container>
      </WSProvider>
    </GSProvider>
  )
}