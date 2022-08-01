import React, { SetStateAction } from 'react'
import styled from 'styled-components';
import { Box } from '@mui/material';
import ReplayImage from './ReplayImage';

type Props = {
  gamemode: string;
  date: string;
  img: string;
  "svg_data": string;
  setSideBarData: () => void;
}

const Container = styled.div`
  width: 320px;
  height: 320px;
  background: var(--accent-dark);
  border-radius: 5px;
  
  display: flex;
  flex-direction: column;
`;

type TagProp = {
  padding?: string;
  color?: string;
}

const Tag = styled.div<TagProp>`
  border-radius: 20px;
  max-height: 25px;
  padding: ${props=> props.padding ? props.padding: "1px 8px"};
  background: ${props => props.color};

  font-size: 13px;
  text-align: center;
`;

// const ImagePlaceholder = styled.div`
//   width: 100%;
//   height: 200px;
//   background: var(--accent-dark);
// `

const BottomTags = styled.div`
  display: flex;
  padding: 10px 30px;
`;

export default function ReplayPreview(props: Props) {
  const { gamemode, date, svg_data, setSideBarData } = props

  return (
    <Container onClick={setSideBarData}>
      <Box display="flex" justifyContent="flexStart" margin="10px">
        <Tag color="grey">{gamemode}</Tag>
      </Box>
      <ReplayImage svg_data={svg_data}/>
    </Container>
  )
}