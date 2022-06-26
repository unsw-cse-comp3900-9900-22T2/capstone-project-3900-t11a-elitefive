import React, { useEffect, useRef } from 'react'
import styled from 'styled-components';

// local
import Board from '../components/Board';
import YavalathButton from '../components/YavalathButton';
import { GSProvider } from '../global/GlobalGameState';
import { WSProvider } from '../global/GlobalWS';

type Props = {}

const Container = styled.div`

`;

export default function Gamepage({}: Props) {

  return (
    <GSProvider>
      <WSProvider>
        <Container>
          <YavalathButton/>
          <Board />
        </Container> 
      </WSProvider>
    </GSProvider>
  )
}