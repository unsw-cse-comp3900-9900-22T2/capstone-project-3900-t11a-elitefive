import React, { useEffect, useRef, useState } from 'react'
import styled from 'styled-components';
import Dropdown from './Dropdown';
import StyledInput from './StyledInput';

type Props = {}

const Container = styled.div`
  margin-top: 10px;
  position: absolute;
  display: flex;
  gap: 50px;
`;

const filterSelections = [
  "elo",
  "mode",
]

export default function FilterBar({}: Props) {

  const [filter, setFilter] = useState<string|undefined>();

  return (
    <Container>
      <StyledInput size="small"/>
      <Dropdown 
        selected={filter}
        selections={filterSelections} 
        setSelected={(selection: string) => {
          setFilter(selection);
        }}
      />
    </Container>
  )
}