import React from 'react'
import styled from 'styled-components';
import Dropdown from './Dropdown';
import StyledInput from './StyledInput';

type Props = {}

const Container = styled.div`

`;

export default function FilterBar({}: Props) {
  return (
    <Container>
      <StyledInput size="small"/>
      <Dropdown />
    </Container>
  )
}