import React from 'react'
import styled from 'styled-components';

// local
import Board from '../components/Board';
import YavalathButton from '../components/YavalathButton';

type Props = {}

const Container = styled.div`

`;

export default function Gamepage({}: Props) {
  return (
    <Container>
      <YavalathButton/>
      <Board />
    </Container>
  )
}