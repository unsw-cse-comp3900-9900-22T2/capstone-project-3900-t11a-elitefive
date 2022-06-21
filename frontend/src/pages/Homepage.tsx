import React from 'react'
import HexCell from '../components/HexCell'

type Props = {}

export default function Homepage({}: Props) {
  return (
    <div>
      <HexCell width={110} height={120}/>
    </div>
  )
}