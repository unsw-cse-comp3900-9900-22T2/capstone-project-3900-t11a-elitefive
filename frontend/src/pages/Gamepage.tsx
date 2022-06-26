import React, { useEffect, useRef } from 'react'
import styled from 'styled-components';
import WSClient from '../api/ws/client';

// local
import Board from '../components/Board';
import YavalathButton from '../components/YavalathButton';

type Props = {}

const Container = styled.div`

`;

export default function Gamepage({}: Props) {
  const WSRef = useRef<WSClient | null>(null);
  useEffect(() => {
    WSRef.current = new WSClient();
  },[])
  // testing
  return (
    <Container>
      <YavalathButton/>
      <Board />
      <button onClick={() => {
        WSRef.current?.emit("moves", JSON.stringify({
          'uid': "abcd",
          'move': 'a1'
        }));
      }}>click me</button>
    </Container> 
  )
}