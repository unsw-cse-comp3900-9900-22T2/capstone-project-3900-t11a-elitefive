import React, { useEffect, useRef } from 'react'
import styled from 'styled-components';

// local
import Board from '../components/Board';
import YavalathButton from '../components/YavalathButton';
import { WSProvider } from '../global/GlobalWS';

type Props = {}

const Container = styled.div`

`;

export default function Gamepage({}: Props) {
  // const WSRef = useRef<WSClient | null>(null);
  // useEffect(() => {
  //   WSRef.current = new WSClient();
  // },[])
  // testing
  return (
    <WSProvider>
      <Container>
        <YavalathButton/>
        <Board />
      </Container> 
    </WSProvider>
  )
}