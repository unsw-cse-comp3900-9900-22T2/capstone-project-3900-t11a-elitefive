import React, { useEffect, useRef, useState } from 'react'
import styled from 'styled-components';

// local
import { GSProvider, useGameState } from '../../global/GlobalGameState';
import { WSProvider } from '../../global/GlobalWS';
import Board from '../../components/Board';
import YavalathButton from '../../components/YavalathButton';

import Sidebar from './ReplaySidebar';

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



export default function ReplayGamepage({}: Props) {
  return (
    // <GSProvider>
    //   <WSProvider>
    //     <Container>
    //       <YavalathButton/>
    //       <Container2>
    //         <Board />
    //         <Sidebar />
    //       </Container2>
    //     </Container>
    //   </WSProvider>
    // </GSProvider>
    <></>
  )
}